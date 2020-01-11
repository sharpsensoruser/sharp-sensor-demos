/////////////////////////////////////////////////////////////////////////////
// SharpMicrowaveSensorDemo
//   Form1.cs
//
// Board Connection:
//   DC6S4ZN3101   Serial-to-USB
//   VCC           3.3V          
//   GND           GND
//   TX            RX
//   RX            TX
//
// Serial communication settings:
//   115200 baud, 8, N, 1
/////////////////////////////////////////////////////////////////////////////

using System;
using System.IO;
using System.IO.Ports;
using System.Threading;
using System.Windows.Forms;

namespace SharpMicrowaveSensorDemo
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
        private const int PAYLOAD_TYPE_IQ = 1;
        private const int PAYLOAD_TYPE_SIGNALMEAN = 5;
        private const int PAYLOAD_TYPE_DEBUG = 7;
        private const int PAYLOAD_TYPE_ALARM = 11;

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
        /// Calculate the checksum by XOR'ing all of the bytes in the Value field of the Payload, 
        /// and then XOR'ing the result with 0xff.
        /// </summary>
        /// <param name="buf">Buffer containing the bytes of the Value field.</param>
        /// <param name="len">The number of bytes comprising the Value field.</param>
        /// <returns></returns>
        byte CalculateChecksum(byte[] buf, int len)
        {
            byte checksum = 0xff;
            for(int i = 0; i < len; i++)
            {
                checksum ^= buf[i];
            }
            return checksum;
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

                if (data >= 0 && this.logEachByteCheckBox.Checked)
                {
                    WriteToLog("Read: (0x" + data.ToString("x2") + ")");
                }
            }
            catch
            { }

            return data;
        }

        /// <summary>
        /// Read Payload portion of data packet.
        /// </summary>
        /// <param name="buf">Byte buffer to read into.</param>
        /// <param name="len">Number of bytes to read.</param>
        /// <returns></returns>
        private bool ReadPayloadValue(byte[] buf, int len)
        {
            for (int i = 0; i < len; i++)
            {
                int data = ReadSerialByte();
                if (data < 0)
                    return false;
                buf[i] = (byte)data;
            }
            return true;
        }

        /// <summary>
        /// Read rest of IQ data packet following the Type field.
        /// </summary>
        private void ReadIQPacket()
        {
            // Read payload length. Length should be 4 bytes.
            int payloadLength = ReadSerialByte();
            if (payloadLength != 4)
                return;

            // Read payload value.
            byte[] buf = new byte[4];
            if (!ReadPayloadValue(buf, 4))
                return;
            int waveI = (int)buf[0] * 256 + (int)buf[1];
            if (waveI > 32767)
            {
                waveI -= 65536;
            }
            int waveQ = (int)buf[2] * 256 + (int)buf[3];
            if (waveQ > 32767)
            {
                waveQ -= 65536;
            }

            // Read sequence number.
            int sequence = ReadSerialByte();
            if (sequence < 0 || sequence > 127)
                return;

            // Read checksum and verify it.
            int checksum = ReadSerialByte();
            int calcChecksum = CalculateChecksum(buf, 4);
            if (checksum != calcChecksum)
                return;

            // Write to log now that data packet is validated.
            if (this.logIQPacketsCheckBox.Checked)
            {
                string text = "Type=" + PAYLOAD_TYPE_IQ;
                text += ", Length=" + payloadLength;
                text += ", WaveI=" + waveI;
                text += ", WaveQ=" + waveQ;
                text += ", Sequence=" + sequence;
                text += ", Checksum=" + checksum;
                WriteToLog(text);
            }
            if (this.logIQDataValuesCheckBox.Checked)
            {
                string text = waveI + "," + waveQ;
                WriteToLog(text);
            }
        }

        /// <summary>
        /// Read the rest of the Signal Mean data packet following the Type field.
        /// </summary>
        private void ReadSignalMeanPacket()
        {
            // Read payload length. Length should be 2 bytes.
            int payloadLength = ReadSerialByte();
            if (payloadLength != 2)
                return;

            // Read payload value.
            byte[] buf = new byte[2];
            if (!ReadPayloadValue(buf, 2))
                return;
            int payloadValue = (int)buf[0] * 256 + (int)buf[1];

            // Read sequence number.
            int sequence = ReadSerialByte();
            if (sequence != 0)
                return;

            // Read checksum and verify it.
            int checksum = ReadSerialByte();
            int calcChecksum = CalculateChecksum(buf, 2);
            if (checksum != calcChecksum)
                return;

            // Write to log now that data packet is validated.
            if (this.logSignalMeanPacketsCheckBox.Checked)
            {
                string text = "Type=" + PAYLOAD_TYPE_SIGNALMEAN;
                text += ", Length=" + payloadLength;
                text += ", SignalMeanValue=" + payloadValue;
                text += ", Sequence=" + sequence;
                text += ", Checksum=" + checksum;
                WriteToLog(text);
            }
        }

        /// <summary>
        /// Read the rest of the Debug packet following the Type field.
        /// </summary>
        private void ReadDebugPacket()
        {
            // Read payload length which should be from 1 to 32 bytes.
            int payloadLength = ReadSerialByte();
            if (payloadLength < 1 || payloadLength > 32)
                return;

            // Read payload value.
            byte[] buf = new byte[33];
            for(int i = 0; i < buf.Length; i++)
            {
                buf[i] = 0;
            }
            if (!ReadPayloadValue(buf, payloadLength))
                return;

            // Write to log.
            if (this.logDebugPacketsCheckBox.Checked)
            {
                string text = "Type=" + PAYLOAD_TYPE_DEBUG;
                text += ", Length=" + payloadLength + ", DebugInfo=";
                for (int i = 0; i < buf.Length; i++)
                {
                    if (buf[i] == 0 || buf[i] == 0x0d)
                        break;
                    text += (char)buf[i];
                }
                WriteToLog(text);
            }
        }

        /// <summary>
        /// Read rest of Alarm packet following the Type field.
        /// </summary>
        void ReadAlarmPacket()
        {
            // Read payload length. Length should be 2 bytes.
            int payloadLength = ReadSerialByte();
            if (payloadLength != 2)
                return;

            // Read payload value.
            byte[] buf = new byte[2];
            if (!ReadPayloadValue(buf, 2))
                return;
            int alarm0 = (buf[0] & 0xf0) / 16;
            int alarm1 = (buf[0] & 0x0f);
            int alarm2 = (buf[1] & 0xf0) / 16;
            int alarm3 = (buf[1] & 0x0f);
            if (alarm0 > 1 || alarm1 > 1 || alarm2 > 1 || alarm3 > 1)
                return;

            // Read sequence number.
            int sequence = ReadSerialByte();
            if (sequence != 0)
                return;

            // Read checksum and verify it.
            int checksum = ReadSerialByte();
            int calcChecksum = CalculateChecksum(buf, 2);
            if (checksum != calcChecksum)
                return;

            // Write to log now that data packet is validated.
            if (this.logAlarmPacketsCheckBox.Checked)
            {
                string text = "Type=" + PAYLOAD_TYPE_ALARM;
                text += ", Length=" + payloadLength;
                text += ", Alarm0=" + alarm0;
                text += ", Alarm1=" + alarm1;
                text += ", Alarm2=" + alarm2;
                text += ", Alarm3=" + alarm3;
                text += ", Sequence=" + sequence;
                text += ", Checksum=" + checksum;
                WriteToLog(text);
            }
        }

        /// <summary>
        /// Write text to the serial COM port.
        /// </summary>
        /// <param name="text">Text to write to serial.</param>
        private void WriteSerial(string text)
        {
            if (!this.serialPort.IsOpen)
                return;
            if (text.Length == 0)
                return;
            this.serialPort.Write(text);

            // Write a carriage return character.
            byte[] buf = new byte[1];
            buf[0] = 0x0d;
            this.serialPort.Write(buf, 0, 1);
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
                    WriteToLog("Send: " + this.serialWriteText);
                    WriteSerial(this.serialWriteText);
                    FlushLogFile();
                    this.serialWriteText = string.Empty;
                    continue;
                }

                // Read payload type and handle accordingly.
                int payloadType = ReadSerialByte();                
                if (payloadType == PAYLOAD_TYPE_IQ)
                {
                    ReadIQPacket();
                }
                else if (payloadType == PAYLOAD_TYPE_SIGNALMEAN)
                {
                    ReadSignalMeanPacket();
                }
                else if (payloadType == PAYLOAD_TYPE_DEBUG)
                {
                    ReadDebugPacket();
                }
                else if (payloadType == PAYLOAD_TYPE_ALARM)
                {
                    ReadAlarmPacket();
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
                    if (this.writeCount >= 100)
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
                this.serialPort.BaudRate = 115200;
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
            if ( this.sendTextBox.TextLength == 0 )
            {
                MessageBox.Show("You must enter some text to send.", "Send", MessageBoxButtons.OK);
                return;
            }

            // Set the send text.
            this.serialWriteText = this.sendTextBox.Text;
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
