namespace SharpMicrowaveSensorDemo
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.disconnectButton = new System.Windows.Forms.Button();
            this.connectButton = new System.Windows.Forms.Button();
            this.portsComboBox = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.sendButton = new System.Windows.Forms.Button();
            this.sendTextBox = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.logEachByteCheckBox = new System.Windows.Forms.CheckBox();
            this.logIQDataValuesCheckBox = new System.Windows.Forms.CheckBox();
            this.logTimestampCheckBox = new System.Windows.Forms.CheckBox();
            this.clearButton = new System.Windows.Forms.Button();
            this.logTextBox = new System.Windows.Forms.TextBox();
            this.logAlarmPacketsCheckBox = new System.Windows.Forms.CheckBox();
            this.logDebugPacketsCheckBox = new System.Windows.Forms.CheckBox();
            this.logSignalMeanPacketsCheckBox = new System.Windows.Forms.CheckBox();
            this.logIQPacketsCheckBox = new System.Windows.Forms.CheckBox();
            this.logFileNameTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.disconnectButton);
            this.groupBox1.Controls.Add(this.connectButton);
            this.groupBox1.Controls.Add(this.portsComboBox);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(615, 57);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Serial Connection";
            // 
            // disconnectButton
            // 
            this.disconnectButton.Location = new System.Drawing.Point(335, 21);
            this.disconnectButton.Name = "disconnectButton";
            this.disconnectButton.Size = new System.Drawing.Size(75, 23);
            this.disconnectButton.TabIndex = 3;
            this.disconnectButton.Text = "Disconnect";
            this.disconnectButton.UseVisualStyleBackColor = true;
            this.disconnectButton.Click += new System.EventHandler(this.disconnectButton_Click);
            // 
            // connectButton
            // 
            this.connectButton.Location = new System.Drawing.Point(254, 21);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(75, 23);
            this.connectButton.TabIndex = 2;
            this.connectButton.Text = "Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // portsComboBox
            // 
            this.portsComboBox.FormattingEnabled = true;
            this.portsComboBox.Location = new System.Drawing.Point(83, 22);
            this.portsComboBox.Name = "portsComboBox";
            this.portsComboBox.Size = new System.Drawing.Size(165, 21);
            this.portsComboBox.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(22, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(55, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "COM port:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.sendButton);
            this.groupBox2.Controls.Add(this.sendTextBox);
            this.groupBox2.Location = new System.Drawing.Point(12, 75);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(615, 72);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Transmit";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(23, 49);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(316, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "(ex. wave off/100/500, ver, th0 400, th1 950, on2tm 5, off3tm 10)";
            // 
            // sendButton
            // 
            this.sendButton.Location = new System.Drawing.Point(335, 19);
            this.sendButton.Name = "sendButton";
            this.sendButton.Size = new System.Drawing.Size(75, 23);
            this.sendButton.TabIndex = 4;
            this.sendButton.Text = "Send";
            this.sendButton.UseVisualStyleBackColor = true;
            this.sendButton.Click += new System.EventHandler(this.sendButton_Click);
            // 
            // sendTextBox
            // 
            this.sendTextBox.Location = new System.Drawing.Point(25, 21);
            this.sendTextBox.Name = "sendTextBox";
            this.sendTextBox.Size = new System.Drawing.Size(304, 20);
            this.sendTextBox.TabIndex = 0;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.logEachByteCheckBox);
            this.groupBox3.Controls.Add(this.logIQDataValuesCheckBox);
            this.groupBox3.Controls.Add(this.logTimestampCheckBox);
            this.groupBox3.Controls.Add(this.clearButton);
            this.groupBox3.Controls.Add(this.logTextBox);
            this.groupBox3.Controls.Add(this.logAlarmPacketsCheckBox);
            this.groupBox3.Controls.Add(this.logDebugPacketsCheckBox);
            this.groupBox3.Controls.Add(this.logSignalMeanPacketsCheckBox);
            this.groupBox3.Controls.Add(this.logIQPacketsCheckBox);
            this.groupBox3.Controls.Add(this.logFileNameTextBox);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Location = new System.Drawing.Point(12, 153);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(615, 361);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Receive";
            // 
            // logEachByteCheckBox
            // 
            this.logEachByteCheckBox.AutoSize = true;
            this.logEachByteCheckBox.Checked = true;
            this.logEachByteCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.logEachByteCheckBox.Location = new System.Drawing.Point(25, 122);
            this.logEachByteCheckBox.Name = "logEachByteCheckBox";
            this.logEachByteCheckBox.Size = new System.Drawing.Size(120, 17);
            this.logEachByteCheckBox.TabIndex = 11;
            this.logEachByteCheckBox.Text = "Log each byte (hex)";
            this.logEachByteCheckBox.UseVisualStyleBackColor = true;
            // 
            // logIQDataValuesCheckBox
            // 
            this.logIQDataValuesCheckBox.AutoSize = true;
            this.logIQDataValuesCheckBox.Location = new System.Drawing.Point(437, 92);
            this.logIQDataValuesCheckBox.Name = "logIQDataValuesCheckBox";
            this.logIQDataValuesCheckBox.Size = new System.Drawing.Size(116, 17);
            this.logIQDataValuesCheckBox.TabIndex = 10;
            this.logIQDataValuesCheckBox.Text = "Log IQ data values";
            this.logIQDataValuesCheckBox.UseVisualStyleBackColor = true;
            // 
            // logTimestampCheckBox
            // 
            this.logTimestampCheckBox.AutoSize = true;
            this.logTimestampCheckBox.Location = new System.Drawing.Point(218, 92);
            this.logTimestampCheckBox.Name = "logTimestampCheckBox";
            this.logTimestampCheckBox.Size = new System.Drawing.Size(94, 17);
            this.logTimestampCheckBox.TabIndex = 9;
            this.logTimestampCheckBox.Text = "Log timestamp";
            this.logTimestampCheckBox.UseVisualStyleBackColor = true;
            // 
            // clearButton
            // 
            this.clearButton.Location = new System.Drawing.Point(522, 131);
            this.clearButton.Name = "clearButton";
            this.clearButton.Size = new System.Drawing.Size(75, 23);
            this.clearButton.TabIndex = 8;
            this.clearButton.Text = "Clear Text";
            this.clearButton.UseVisualStyleBackColor = true;
            this.clearButton.Click += new System.EventHandler(this.clearButton_Click);
            // 
            // logTextBox
            // 
            this.logTextBox.Location = new System.Drawing.Point(25, 160);
            this.logTextBox.Multiline = true;
            this.logTextBox.Name = "logTextBox";
            this.logTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.logTextBox.Size = new System.Drawing.Size(572, 183);
            this.logTextBox.TabIndex = 7;
            // 
            // logAlarmPacketsCheckBox
            // 
            this.logAlarmPacketsCheckBox.AutoSize = true;
            this.logAlarmPacketsCheckBox.Checked = true;
            this.logAlarmPacketsCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.logAlarmPacketsCheckBox.Location = new System.Drawing.Point(25, 92);
            this.logAlarmPacketsCheckBox.Name = "logAlarmPacketsCheckBox";
            this.logAlarmPacketsCheckBox.Size = new System.Drawing.Size(161, 17);
            this.logAlarmPacketsCheckBox.TabIndex = 6;
            this.logAlarmPacketsCheckBox.Text = "Log alarm packets (Type 11)";
            this.logAlarmPacketsCheckBox.UseVisualStyleBackColor = true;
            // 
            // logDebugPacketsCheckBox
            // 
            this.logDebugPacketsCheckBox.AutoSize = true;
            this.logDebugPacketsCheckBox.Checked = true;
            this.logDebugPacketsCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.logDebugPacketsCheckBox.Location = new System.Drawing.Point(437, 63);
            this.logDebugPacketsCheckBox.Name = "logDebugPacketsCheckBox";
            this.logDebugPacketsCheckBox.Size = new System.Drawing.Size(160, 17);
            this.logDebugPacketsCheckBox.TabIndex = 5;
            this.logDebugPacketsCheckBox.Text = "Log debug packets (Type 7)";
            this.logDebugPacketsCheckBox.UseVisualStyleBackColor = true;
            // 
            // logSignalMeanPacketsCheckBox
            // 
            this.logSignalMeanPacketsCheckBox.AutoSize = true;
            this.logSignalMeanPacketsCheckBox.Checked = true;
            this.logSignalMeanPacketsCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.logSignalMeanPacketsCheckBox.Location = new System.Drawing.Point(218, 63);
            this.logSignalMeanPacketsCheckBox.Name = "logSignalMeanPacketsCheckBox";
            this.logSignalMeanPacketsCheckBox.Size = new System.Drawing.Size(186, 17);
            this.logSignalMeanPacketsCheckBox.TabIndex = 4;
            this.logSignalMeanPacketsCheckBox.Text = "Log signal mean packets (Type 5)";
            this.logSignalMeanPacketsCheckBox.UseVisualStyleBackColor = true;
            // 
            // logIQPacketsCheckBox
            // 
            this.logIQPacketsCheckBox.AutoSize = true;
            this.logIQPacketsCheckBox.Location = new System.Drawing.Point(25, 63);
            this.logIQPacketsCheckBox.Name = "logIQPacketsCheckBox";
            this.logIQPacketsCheckBox.Size = new System.Drawing.Size(141, 17);
            this.logIQPacketsCheckBox.TabIndex = 3;
            this.logIQPacketsCheckBox.Text = "Log IQ packets (Type 1)";
            this.logIQPacketsCheckBox.UseVisualStyleBackColor = true;
            // 
            // logFileNameTextBox
            // 
            this.logFileNameTextBox.Location = new System.Drawing.Point(76, 25);
            this.logFileNameTextBox.Name = "logFileNameTextBox";
            this.logFileNameTextBox.ReadOnly = true;
            this.logFileNameTextBox.Size = new System.Drawing.Size(334, 20);
            this.logFileNameTextBox.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 29);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Log File:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(639, 523);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "SharpMicrowaveSensorDemo";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox portsComboBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button disconnectButton;
        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button sendButton;
        private System.Windows.Forms.TextBox sendTextBox;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox logFileNameTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button clearButton;
        private System.Windows.Forms.TextBox logTextBox;
        private System.Windows.Forms.CheckBox logAlarmPacketsCheckBox;
        private System.Windows.Forms.CheckBox logDebugPacketsCheckBox;
        private System.Windows.Forms.CheckBox logSignalMeanPacketsCheckBox;
        private System.Windows.Forms.CheckBox logIQPacketsCheckBox;
        private System.Windows.Forms.CheckBox logTimestampCheckBox;
        private System.Windows.Forms.CheckBox logEachByteCheckBox;
        private System.Windows.Forms.CheckBox logIQDataValuesCheckBox;
        private System.Windows.Forms.Label label3;
    }
}

