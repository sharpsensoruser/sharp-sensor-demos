/////////////////////////////////////////////////////////////////////////////
// FoxconnTofModuleDemo (UART)
//   Form1.cs
//
// Board Connection:
//   Serial-to-USB   FSTOF2002C0U / FSTOF2002C0D / FSTOF2003C0U
//   3.3V            Pin 1 - VDD
//   GND             Pin 2 - GND
//   3.3V RX         Pin 3 - TXD
//   3.3V TX         Pin 4 - RXD
//
// Serial communication settings:
//   9600 baud, 8, N, 1
/////////////////////////////////////////////////////////////////////////////

using System;
using System.Globalization;
using System.IO;
using System.IO.Ports;
using System.Windows.Forms;

namespace FoxconnTofModuleDemo
{
    // UART command codes.
    enum UartCommandCodes
    {
        TOFM_CMD_ST_MM             = 0x81,  // Start a distance measurement.
        TOFM_CMD_CALI_XTALK        = 0x82,  // Perform crosstalk calibration.
        TOFM_CMD_CALI_OFS          = 0x83,  // Perform offset calibration.
        TOFM_CMD_RESET             = 0x84,  // Reset tof module.
        TOFM_CMD_RD_FACTORY_DATA   = 0x85,  // Read factory data.
        TOFM_CMD_RD_VERSION_INFO   = 0x86,  // Read version info.
        TOFM_CMD_RD_DEBUG_PARA1    = 0x8A,  // Read debug parameters section 1. 
        TOFM_CMD_RD_DEBUG_PARA2    = 0x8B,  // Read debug parameters section 2. 
        TOFM_CMD_RD_DEBUG_PARA3    = 0x8C,  // Read debug parameters section 3. 
        TOFM_CMD_WR_THRESH_INT_OUT = 0x91,  // Set threshold of interrupt output.
        TOFM_CMD_WR_RANGING_MODE   = 0x92   // Set ranging mode.
    };

    // Range1 status values (same as Sharp GP2AP02VT00F TOF Sensor).
    enum Range1Status
    {
        RANGE1_STATUS_VALID_DATA      = 0x00,
        RANGE1_STATUS_VCSEL_SHORT     = 0x01,
        RANGE1_STATUS_LOW_SIGNAL      = 0x02,
        RANGE1_STATUS_LOW_SN          = 0x04,
        RANGE1_STATUS_TOO_MUCH_AMB    = 0x08,
        RANGE1_STATUS_WAF             = 0x10,
        RANGE1_STATUS_CAL_ERROR       = 0x20,
        RANGE1_STATUS_CROSSTALK_ERROR = 0x80
    };
    
    // Main application screen.
    public partial class Form1 : Form
    {
        
        #region Constructor
        /// <summary>
        /// Initialize the main form.
        /// </summary>
        public Form1()
        {
            InitializeComponent();

            // Create a new serial port and use it to populate combo box.
            this.serialPort = new SerialPort();
            foreach(string portName in SerialPort.GetPortNames())
            {
                this.portsComboBox.Items.Add(portName);
            }
            if ( this.portsComboBox.Items.Count > 0 )
            {
                this.portsComboBox.SelectedIndex = this.portsComboBox.Items.Count - 1;
            }

            // Start the timer.
            this.timer.Interval = 500;
            this.timer.Tick += new EventHandler(timer_Tick);
            this.timer.Start();
        }
        #endregion Constructor

        #region Private members
        private const int START_PACKET_1  = 0x55;   // 1st header byte of data packet.
        private const int START_PACKET_2  = 0xAA;   // 2nd header byte of data packet.
        private const int MAX_DATA_LENGTH = 27;     // Data length can vary from 0 bytes to 27 bytes.
        private const int END_PACKET      = 0xFA;   // End byte of data packet.

        private System.Windows.Forms.Timer timer = new System.Windows.Forms.Timer();

        private SerialPort serialPort = null;
        private StreamWriter logFile = null;       
        private string serialWriteText = string.Empty;
        private int writeCount = 0;
        #endregion Private members

        #region Private methods
        /// <summary>
        /// Get the current date and time down to the millisecond.
        /// Used to construct the filename for the log file.
        /// </summary>
        /// <returns>A date timestamp string accurate to the millisecond.</returns>
        private string GetCurrentDateTimestamp()
        {
            string timestamp = DateTime.Now.ToString("yyyy.MM.dd-HH.mm.ss.fff");
            return timestamp;
        }

        /// <summary>
        /// Get the current time down to the millisecond.
        /// Used for logging purposes.
        /// </summary>
        /// <returns>A timestamp string accurate to the millisecond.</returns>
        private string GetCurrentTimestamp()
        {
            string timestamp = DateTime.Now.ToString("HH:mm:ss.fff");
            return timestamp;
        }

        /// <summary>
        /// Create the log file for writing into.
        /// </summary>
        private void OpenLogFile()
        {
            // Compose log file name based on current time.
            string logFileName = @".\_logFile-" + GetCurrentDateTimestamp() + ".txt";

            // Create the log file.
            try
            {
                logFile = new StreamWriter(logFileName);
            }
            catch (Exception e)
            {
                MessageBox.Show("Unable to create log file: " + e.Message, "Create Log File", MessageBoxButtons.OK);
                return;
            }
            this.logFileNameTextBox.Text = logFileName;
        }

        /// <summary>
        /// Flush the log file.
        /// </summary>
        private void FlushLogFile()
        {
            if (logFile == null)
                return;
            try
            {
                logFile.Flush();
            }
            catch
            { }
        }

        /// <summary>
        /// Close the log file.
        /// </summary>
        private void CloseLogFile()
        {
            if (logFile == null)
                return;
            try
            {
                logFile.Close();
            }
            catch
            { }
            logFile = null;
            this.logFileNameTextBox.Text = string.Empty;
        }

        /// <summary>
        /// Read a single byte from the serial COM port.
        /// </summary>
        /// <returns>A single byte or -1 if there was an error.</returns>
        private int ReadSerialByte()
        {
            int data = -1;
            try
            {
                byte[] buf = new byte[1];
                this.serialPort.Read(buf, 0, 1);
                data = buf[0];
            }
            catch
            { }

            return data;
        }

        /// <summary>
        /// Read data packet.
        /// </summary>
        /// <returns>true if successful; false if unexpected data encountered.</returns>
        private bool ReadDataPacket()
        {
            // Look for the 1st header byte of data packet.
            int data = ReadSerialByte();
            if (data != START_PACKET_1)
                return false;

            // Look for the 2nd header byte of data packet.
            data = ReadSerialByte();
            if (data != START_PACKET_2)
                return false;

            // Look for the Command Code.
            int cmdCode = ReadSerialByte();
            if (cmdCode < (int)UartCommandCodes.TOFM_CMD_ST_MM)
                return false;

            // Read the Data Length.
            int dataLen = ReadSerialByte();
            if (dataLen < 0 || dataLen > MAX_DATA_LENGTH)
                return false;

            // Read the Data.
            byte[] dataBuf = new byte[MAX_DATA_LENGTH];
            for(int i = 0; i < dataLen; i++)
            {
                dataBuf[i] = (byte)ReadSerialByte();
            }

            // Read the end byte of the data packet.
            data = ReadSerialByte();
            if (data != END_PACKET)
                return false;

            // Write to log the raw data packet.
            WriteToLog("---");
            string text = "Read Data Packet (HEX):";
            text += (" " + START_PACKET_1.ToString("X2"));
            text += (" " + START_PACKET_2.ToString("X2"));
            text += (" " + cmdCode.ToString("X2"));
            text += (" " + dataLen.ToString("X2"));
            for (int i = 0; i < dataLen; i++)
            {
                text += (" " + dataBuf[i].ToString("X2"));
            }
            text += (" " + END_PACKET.ToString("X2"));
            WriteToLog(text);
            
            // Write to log depending on the type of command.
            if (cmdCode == (int)UartCommandCodes.TOFM_CMD_ST_MM)
            {
                // Print distance and range1 status.
                int distMM = (int)dataBuf[0] * 256 + dataBuf[1];
                WriteToLog("Distance=" + distMM.ToString() + "mm");
                WriteToLog("range1_status=0x" + dataBuf[2].ToString("X2"));
            } 
            else if (cmdCode == (int)UartCommandCodes.TOFM_CMD_RD_VERSION_INFO)
            {
                WriteVersionInfoToLog(dataBuf);
            }
            else if (cmdCode == (int)UartCommandCodes.TOFM_CMD_RD_FACTORY_DATA)
            {
                WriteFactoryDataToLog(dataBuf);
            }

            return true;
        }

        /// <summary>
        /// Extract version info from data buffer and write to log.
        /// </summary>
        /// <param name="dataBuf"></param>
        private void WriteVersionInfoToLog(byte[] dataBuf)
        {
            string text;

            // Print sensor IC model.
            int sensor_ic = dataBuf[0];
            if (sensor_ic == 0x02)
                text = "sensor_ic=0x02 : GP2AP02VT00F";
            else if (sensor_ic == 0x03)
                text = "sensor_ic=0x03 : GP2AP03VT00F";
            else
                text = "sensor_ic=0x" + sensor_ic.ToString("X2");
            WriteToLog(text);

            // Print interface type.
            int port = dataBuf[1];
            if (port == 0x41)
                text = "port=0x41('A') : Firmware supports UART and I2C";
            else if (port == 0x49)
                text = "port=0x49('I') : Firmware supports I2C only";
            else if (port == 0x55)
                text = "port=0x55('U') : Firmware supports UART only";
            else
                text = "port=0x" + port.ToString("X2");
            WriteToLog(text);

            // Print firmware version number.
            int vers = dataBuf[2];
            text = "version=" + vers.ToString();
            WriteToLog(text);
        }

        /// <summary>
        /// Extract factory data which is saved on the flash after calibration and write to log.
        /// </summary>
        /// <param name="dataBuf"></param>
        private void WriteFactoryDataToLog(byte[] dataBuf)
        {
            WriteToLog("offset_short1=0x" + dataBuf[0].ToString("X2"));
            WriteToLog("offset_short2=0x" + dataBuf[1].ToString("X2"));
            WriteToLog("offset_long1=0x" + dataBuf[2].ToString("X2"));
            WriteToLog("offset_long2=0x" + dataBuf[3].ToString("X2"));
            WriteToLog("xtalk_lsb=0x" + dataBuf[4].ToString("X2"));
            WriteToLog("xtalk_msb=0x" + dataBuf[5].ToString("X2"));
            string text;
            int factory_calibrated = dataBuf[6];
            if (factory_calibrated == 0)
                text = "factory_calibrated=0x00 : Calibration is not done";
            else if (factory_calibrated == 1)
                text = "factory_calibrated=0x01 : Offset calibration passed";
            else if (factory_calibrated == 2)
                text = "factory_calibrated=0x02 : Crosstalk calibration passed";
            else if (factory_calibrated == 3)
                text = "factory_calibrated=0x03 : Offset and crosstalk calibration passed";
            else
                text = "factory_calibrated=0x" + factory_calibrated.ToString("X2");
            WriteToLog(text);
        }

        /// <summary>
        /// Write command data to the serial COM port.
        /// </summary>
        /// <param name="text">Text representing the bytes to write to serial.</param>
        /// <returns>true if data was written; false otherwise</returns>
        private bool WriteSerial(string text)
        {
            if (!this.serialPort.IsOpen)
                return false;

            // Do some error checking on the text string. Every cmd/data packet
            // has at least 5 bytes.
            if (text.Length == 0)
                return false;
            string[] textData = text.Split(' ');
            if (textData.Length < 5)
                return false;

            // Parse the hex numbers in the text string.
            byte[] buf = new byte[textData.Length];
            for(int i = 0; i < textData.Length; i++)
            {
                int res;
                if (!int.TryParse(textData[i], System.Globalization.NumberStyles.HexNumber, CultureInfo.CurrentCulture, out res))
                    return false;
                if (res < 0 || res > 255)
                    return false;
                buf[i] = (byte)res;
            }

            // Write the bytes of the data packet to serial.
            this.serialPort.Write(buf, 0, textData.Length);
            return true;
        }

        /// <summary>
        /// Handle the reading and writing to serial COM port.
        /// </summary>
        private void HandleReadWriteSerial()
        {
            // Check if COM port is open.
            if (!this.serialPort.IsOpen)
            {                    
                return;
            }

            // Check if we need to do a Send.
            if (!string.IsNullOrEmpty(this.serialWriteText))
            {
                WriteToLog("---");
                WriteToLog("Send Data Packet (HEX): " + this.serialWriteText);
                if (!WriteSerial(this.serialWriteText))
                {
                    WriteToLog("Send Data Packet (HEX): Invalid data packet format");
                }
                FlushLogFile();
                this.serialWriteText = string.Empty;
                return;
            }

            // Read data packet.
            ReadDataPacket();                

            // Check if we need to send measurement request.
            if (this.continuousMeasurementCheckBox.Checked && string.IsNullOrEmpty(this.serialWriteText))
            {
                this.serialWriteText = "55 AA 81 00 FA";
            }
        }

        /// <summary>
        /// Write a line of text to the log file and to the log textbox.
        /// </summary>
        /// <param name="text">Line of text to log.</param>
        private void WriteToLog(string text)
        {
            // Prepend timestamp if needed.
            string logLine = string.Empty;
            if (this.logTimestampCheckBox.Checked)
                logLine = GetCurrentTimestamp() + ", ";
            logLine += text;

            // Write to log file.
            try
            {
                if (this.logFile != null)
                {
                    this.logFile.WriteLine(logLine);
                    this.writeCount++;
                    if (this.writeCount >= 10)
                    {
                        this.writeCount = 0;
                        FlushLogFile();
                    }
                }
            }
            catch
            { }

            // Write to log textbox.
            this.logTextBox.AppendText(logLine + Environment.NewLine);
        }
        #endregion Private methods

        #region Event handlers
        private void timer_Tick(Object sender, EventArgs e)
        {
            HandleReadWriteSerial();
        }

        private void connectButton_Click(object sender, EventArgs e)
        {
            // Check if the port is already open.
            if ( this.serialPort.IsOpen )
            {
                MessageBox.Show("Serial port is already open.", "Connect", MessageBoxButtons.OK);
                return;
            }

            // Check if there are any COM ports to connect to.
            if ( this.portsComboBox.Items.Count == 0 )
            {
                MessageBox.Show("No COM ports detected.", "Connect", MessageBoxButtons.OK);
                return;
            }

            // Check if a COM port has been selected.
            if (this.portsComboBox.SelectedIndex < 0)
            {
                MessageBox.Show("You must select a COM port first.", "Connect", MessageBoxButtons.OK);
                return;
            }

            try
            {
                // Configure serial port.
                this.serialPort.PortName = this.portsComboBox.SelectedItem.ToString();
                this.serialPort.BaudRate = 9600;
                this.serialPort.Parity = Parity.None;
                this.serialPort.DataBits = 8;
                this.serialPort.StopBits = StopBits.One;
                this.serialPort.Handshake = Handshake.None;
                this.serialPort.ReadBufferSize = 250000000;

                // Set the read/write timeouts.
                this.serialPort.ReadTimeout = 50;
                this.serialPort.WriteTimeout = 250;

                // Open the port.
                this.serialPort.Open();
            }
            catch(Exception ex)
            {
                MessageBox.Show("Error opening COM port: " + ex.Message, "Open COM Port", MessageBoxButtons.OK);
            }

            // Create the log file.
            OpenLogFile();
        }

        private void disconnectButton_Click(object sender, EventArgs e)
        {
            // Check if the port is open.
            if ( !this.serialPort.IsOpen )
            {
                MessageBox.Show("The port is not open. There is nothing to disconnect.", "Disconnect", MessageBoxButtons.OK);
                return;
            }

            // Close the port.
            try
            {
                this.serialPort.Close();
            }
            catch(Exception ex)
            {
                MessageBox.Show("Error closing COM port: " + ex.Message, "Close COM Port", MessageBoxButtons.OK);
            }

            // Close the log file.
            CloseLogFile();
        }

        private void sendButton_Click(object sender, EventArgs e)
        {
            // Check if the port is open.
            if ( !this.serialPort.IsOpen )
            {
                MessageBox.Show("The port is not open.", "Send", MessageBoxButtons.OK);
                return;
            }

            // Check if there is text to send.
            if ( this.sendTextBox.TextLength == 0 || this.sendTextBox.Text.Trim().Length == 0 )
            {
                MessageBox.Show("You must enter some data to send.", "Send", MessageBoxButtons.OK);
                return;
            }

            // Set the send text.
            this.serialWriteText = this.sendTextBox.Text.Trim();
        }

        private void cmdsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch(this.cmdsComboBox.SelectedIndex)
            {
                case 0:
                    // 0x81 - Start a distance measurement
                    this.sendTextBox.Text = "55 AA 81 00 FA";
                    break;
                case 1:
                    // 0x82 - Perform crosstalk calibration
                    this.sendTextBox.Text = "55 AA 82 00 FA";
                    break;
                case 2:
                    // 0x83 - Perform offset calibration
                    this.sendTextBox.Text = "55 AA 83 00 FA";
                    break;
                case 3:
                    // 0x84 - Reset tof module
                    this.sendTextBox.Text = "55 AA 84 00 FA";
                    break;
                case 4:
                    // 0x85 - Read factory data
                    this.sendTextBox.Text = "55 AA 85 00 FA";
                    break;
                case 5:
                    // 0x86 - Read version info
                    this.sendTextBox.Text = "55 AA 86 00 FA";
                    break;
                case 6:
                    // 0x8A - Read debug parameters section 1
                    this.sendTextBox.Text = "55 AA 8A 00 FA";
                    break;
                case 7:
                    // 0x8B - Read debug parameters section 2
                    this.sendTextBox.Text = "55 AA 8B 00 FA";
                    break;
                case 8:
                    // 0x8C - Read debug parameters section 3
                    this.sendTextBox.Text = "55 AA 8C 00 FA";
                    break;
                case 9:
                    // 0x91 - Set threshold of interrupt output
                    this.sendTextBox.Text = "55 AA 91 04 64 00 E8 03 FA";
                    break;
                case 10:
                    // 0x92 - Set ranging mode
                    this.sendTextBox.Text = "55 AA 92 01 01 FA";
                    break;
                default:
                    // Clear text box
                    this.sendTextBox.Text = "";
                    break;
            }
        }

        private void clearButton_Click(object sender, EventArgs e)
        {
            // Clear the log textbox contents.
            this.logTextBox.ResetText();

            // Flush the log file.
            FlushLogFile();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Stop the timer.
            this.timer.Stop();

            // Flush and then close the log file.
            FlushLogFile();
            CloseLogFile();
        }
        #endregion Event handlers
    }
}
