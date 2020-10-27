/////////////////////////////////////////////////////////////////////////////
// SharpDustSensorDemo
//   Form1.cs
//
// Board Connection:
//   Serial-to-USB   GP2Y1040AU0F
//   3.3V RX         Pin 1 - TXD
//   3.3V TX         Pin 2 - RXD
//   Not Connected   Pin 3 - SEL
//   GND             Pin 4 - GND
//   5V              Pin 5 - VCC
//   GND             Pin 6 - GND
//
// Serial communication settings:
//   9600 baud, 8, N, 1
/////////////////////////////////////////////////////////////////////////////

using System;
using System.Globalization;
using System.IO;
using System.IO.Ports;
using System.Threading;
using System.Windows.Forms;

namespace SharpDustSensorDemo
{
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

            // Start the read write thread.
            this.serialPortThread = new Thread(ReadWriteSerialThread);
            this.serialPortThread.Start();
        }
        #endregion Constructor

        #region Private members
        private const int START_FRAME_1 = 0xFF;
        private const int START_FRAME_2 = 0xFA;
        private const int CMD_FRAME_1 = 0x42;
        private const int CMD_FRAME_2 = 0x4D;

        private SerialPort serialPort = null;
        private StreamWriter logFile = null;
        private Thread serialPortThread = null;
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
        /// Read air quality data record (number and pm concentrations) bytes
        /// following the first start frame.
        /// </summary>
        /// <returns>true if successful; false if unexpected data encountered.</returns>
        private bool ReadAirQualityData()
        {
            byte[] frames = new byte[28];

            // Set the 1st start frame since we already read it.
            frames[0] = START_FRAME_1;

            // Look for the 2nd start frame.
            int data = ReadSerialByte();
            if (data != START_FRAME_2)
                return false;
            frames[1] = (byte)data;

            // Read in 26 remaining bytes from serial.            
            for (int i = 2; i < 28; i++)
            {
                data = ReadSerialByte();
                if (data < 0 || data > 255)
                    return false;
                frames[i] = (byte)data;
            }

            // Write to log the raw air data frames in two lines.
            WriteToLog("---");
            string text = "Read Air Data (HEX):";
            for (int i = 0; i < 14; i++)
            {
                text += (" " + frames[i].ToString("X2"));
            }
            WriteToLog(text);
            text = "Read Air Data (HEX):";
            for (int i = 14; i < 28; i++)
            {
                text += (" " + frames[i].ToString("X2"));
            }
            WriteToLog(text);

            // Compare the checksum.
            byte checksum = 0;
            for (int i = 0; i < 27; i++)
            {
                checksum += frames[i];
            }
            if (checksum != frames[27])
            {
                string msg = "Error: Checksum doesn't match: calculated sum is: 0x" + checksum.ToString("X2");
                WriteToLog(msg);
                return false;
            }

            // Calculate the number and mass concentrations from the raw frame data.
            int NC_03  = frames[2]  * 256 + frames[3];
            int NC_05  = frames[4]  * 256 + frames[5];
            int NC_1   = frames[6]  * 256 + frames[7];
            int NC_25  = frames[8]  * 256 + frames[9];
            int NC_4   = frames[10] * 256 + frames[11];
            int PM1_1  = frames[14] * 256 + frames[15];
            int PM25_1 = frames[16] * 256 + frames[17];
            int PM10_1 = frames[18] * 256 + frames[19];
            int PM1_2  = frames[20] * 256 + frames[21];
            int PM25_2 = frames[22] * 256 + frames[23];
            int PM10_2 = frames[24] * 256 + frames[25];

            // Write to log all the number and mass concentrations.
            text = "NC_03="  +  NC_03.ToString() + ", " +
                   "NC_05="  +  NC_05.ToString() + ", " +
                   "NC_1="   +   NC_1.ToString() + ", " +
                   "NC_25="  +  NC_25.ToString() + ", " +
                   "NC_4="   +   NC_4.ToString();
            WriteToLog(text);
            text = "PM1_1="  +  PM1_1.ToString() + ", " +
                   "PM25_1=" + PM25_1.ToString() + ", " +
                   "PM10_1=" + PM10_1.ToString();
            WriteToLog(text);
            text = "PM1_2="  +  PM1_2.ToString() + ", " +
                   "PM25_2=" + PM25_2.ToString() + ", " +
                   "PM10_2=" + PM10_2.ToString();
            WriteToLog(text);

            return true;
        }

        /// <summary>
        /// Read rest of command record following the first cmd frame.
        /// </summary>
        /// <returns>true if successful; false if unexpected data encountered.</returns>
        private bool ReadCommandData()
        {
            byte[] frames = new byte[7];

            // Set the 1st cmd frame since we already read it.
            frames[0] = CMD_FRAME_1;

            // Look for the 2nd cmd frame.
            int data = ReadSerialByte();
            if (data != CMD_FRAME_2)
                return false;
            frames[1] = (byte)data;

            // Read in 5 remaining bytes from serial.
            for(int i = 2; i < 7; i++)
            {
                data = ReadSerialByte();
                if (data < 0 || data > 255)
                    return false;
                frames[i] = (byte)data;
            }

            // Write to log all 7 bytes of cmd data.
            WriteToLog("---");
            string text = "Read Cmd Data (HEX):";
            for(int i = 0; i < 7; i++)
            {
                text += (" " + frames[i].ToString("X2"));
            }
            WriteToLog(text);
            return true;
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

            // Do some error checking on the text string.
            if (text.Length == 0)
                return false;
            string[] cmdData = text.Split(' ');
            if (cmdData.Length != 7)
                return false;

            // Parse the 7 hex numbers in the text string.
            byte[] buf = new byte[7];
            for(int i = 0; i < 7; i++)
            {
                int res;
                if (!int.TryParse(cmdData[i], System.Globalization.NumberStyles.HexNumber, CultureInfo.CurrentCulture, out res))
                    return false;
                if (res < 0 || res > 255)
                    return false;
                buf[i] = (byte)res;
            }

            // Write the 7 bytes of the command record to serial.
            this.serialPort.Write(buf, 0, 7);
            return true;
        }

        /// <summary>
        /// Implements the thread which does the reading and writing to COM port.
        /// </summary>
        private void ReadWriteSerialThread()
        {
            while (true)
            {
                // Check if COM port is open.
                if (!this.serialPort.IsOpen)
                {
                    Thread.Sleep(500);
                    continue;
                }

                // Check if we need to do a Send.
                if (!string.IsNullOrEmpty(this.serialWriteText))
                {
                    WriteToLog("---");
                    WriteToLog("Send Cmd Data (HEX): " + this.serialWriteText);
                    if (!WriteSerial(this.serialWriteText))
                    {
                        WriteToLog("Send Cmd Data (HEX): Invalid command data format");
                    }
                    FlushLogFile();
                    this.serialWriteText = string.Empty;
                    continue;
                }

                // Read frame and handle accordingly.
                int frame = ReadSerialByte();                
                if (frame == START_FRAME_1)
                {
                    if ( !ReadAirQualityData() )
                    {
                        WriteToLog("Read Air Data (HEX): Invalid air quality data format");
                    }
                }
                else if (frame == CMD_FRAME_1)
                {
                    if ( !ReadCommandData() )
                    {
                        WriteToLog("Read Cmd Data (HEX): Invalid command data format");
                    }
                }
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
                this.serialPort.ReadTimeout = 500;
                this.serialPort.WriteTimeout = 500;

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
                    // READ: Read data in passive mode
                    this.sendTextBox.Text = "42 4D E2 00 00 01 71";
                    break;
                case 1:
                    // MODE: Switch to passive mode
                    this.sendTextBox.Text = "42 4D E1 00 00 01 70";
                    break;
                case 2:
                    // MODE: Switch to active mode
                    this.sendTextBox.Text = "42 4D E1 00 01 01 71";
                    break;
                case 3:
                    // SLEEP: Go to sleep
                    this.sendTextBox.Text = "42 4D E4 00 00 01 73";
                    break;
                case 4:
                    // SLEEP: Wake up
                    this.sendTextBox.Text = "42 4D E4 00 01 01 74";
                    break;
                case 5:
                    // CLEAN: Start cleaning
                    this.sendTextBox.Text = "42 4D 01 00 00 00 90";
                    break;
                case 6:
                    // RESET: Software reset
                    this.sendTextBox.Text = "42 4D 02 81 00 01 12";
                    break;
                case 7:
                    // MAVE: Set number of moving averages to 10
                    this.sendTextBox.Text = "42 4D 03 00 0A 00 9C";
                    break;
                case 8:
                    // TINT: Set interval time to 0 seconds
                    this.sendTextBox.Text = "42 4D 04 00 00 00 93";
                    break;
                case 9:
                    // TPREFAN: Set pre-rotation time of fan to 3 seconds
                    this.sendTextBox.Text = "42 4D 05 00 03 00 97";
                    break;
                case 10:
                    // TINTC: Set interval time for auto-cleaning to 60480[10s]
                    this.sendTextBox.Text = "42 4D 06 EC 40 01 C1";
                    break;
                case 11:
                    // TCLEAN: Set cleaning time to 10 seconds
                    this.sendTextBox.Text = "42 4D 07 00 0A 00 A0";
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
            // Stop the read/write thread.
            if (this.serialPortThread != null)
                this.serialPortThread.Abort();

            // Flush and then close the log file.
            FlushLogFile();
            CloseLogFile();
        }
        #endregion Event handlers
    }
}
