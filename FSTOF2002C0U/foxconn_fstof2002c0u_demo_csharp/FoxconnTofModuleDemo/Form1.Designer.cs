namespace FoxconnTofModuleDemo
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
            this.continuousMeasurementCheckBox = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.cmdsComboBox = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.sendButton = new System.Windows.Forms.Button();
            this.sendTextBox = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.logTimestampCheckBox = new System.Windows.Forms.CheckBox();
            this.clearButton = new System.Windows.Forms.Button();
            this.logTextBox = new System.Windows.Forms.TextBox();
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
            this.groupBox2.Controls.Add(this.continuousMeasurementCheckBox);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.cmdsComboBox);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.sendButton);
            this.groupBox2.Controls.Add(this.sendTextBox);
            this.groupBox2.Location = new System.Drawing.Point(12, 75);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(615, 168);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Transmit UART Command to Tof Module";
            // 
            // continuousMeasurementCheckBox
            // 
            this.continuousMeasurementCheckBox.AutoSize = true;
            this.continuousMeasurementCheckBox.Location = new System.Drawing.Point(25, 27);
            this.continuousMeasurementCheckBox.Name = "continuousMeasurementCheckBox";
            this.continuousMeasurementCheckBox.Size = new System.Drawing.Size(228, 17);
            this.continuousMeasurementCheckBox.TabIndex = 9;
            this.continuousMeasurementCheckBox.Text = "Send measurement command continuously";
            this.continuousMeasurementCheckBox.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(117, 99);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(268, 13);
            this.label5.TabIndex = 8;
            this.label5.Text = "Command format: 55 AA CmdCode DataLen <Data> FA";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(60, 132);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(146, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Select an example command:";
            // 
            // cmdsComboBox
            // 
            this.cmdsComboBox.FormattingEnabled = true;
            this.cmdsComboBox.Items.AddRange(new object[] {
            "0x81 - Start a distance measurement",
            "0x82 - Perform crosstalk calibration",
            "0x83 - Perform offset calibration",
            "0x84 - Reset tof module",
            "0x85 - Read factory data",
            "0x86 - Read version info",
            "0x8A - Read debug parameters section 1",
            "0x8B - Read debug parameters section 2",
            "0x8C - Read debug parameters section 3",
            "0x91 - Set threshold of interrupt output",
            "0x92 - Set ranging mode"});
            this.cmdsComboBox.Location = new System.Drawing.Point(208, 129);
            this.cmdsComboBox.Name = "cmdsComboBox";
            this.cmdsComboBox.Size = new System.Drawing.Size(305, 21);
            this.cmdsComboBox.TabIndex = 6;
            this.cmdsComboBox.SelectedIndexChanged += new System.EventHandler(this.cmdsComboBox_SelectedIndexChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(22, 64);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(187, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Enter cmd bytes separated by spaces:";
            // 
            // sendButton
            // 
            this.sendButton.Location = new System.Drawing.Point(519, 59);
            this.sendButton.Name = "sendButton";
            this.sendButton.Size = new System.Drawing.Size(75, 23);
            this.sendButton.TabIndex = 4;
            this.sendButton.Text = "Send";
            this.sendButton.UseVisualStyleBackColor = true;
            this.sendButton.Click += new System.EventHandler(this.sendButton_Click);
            // 
            // sendTextBox
            // 
            this.sendTextBox.Location = new System.Drawing.Point(209, 61);
            this.sendTextBox.Name = "sendTextBox";
            this.sendTextBox.Size = new System.Drawing.Size(304, 20);
            this.sendTextBox.TabIndex = 0;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.logTimestampCheckBox);
            this.groupBox3.Controls.Add(this.clearButton);
            this.groupBox3.Controls.Add(this.logTextBox);
            this.groupBox3.Controls.Add(this.logFileNameTextBox);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Location = new System.Drawing.Point(12, 250);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(615, 299);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Receive UART Data from TofModule";
            // 
            // logTimestampCheckBox
            // 
            this.logTimestampCheckBox.AutoSize = true;
            this.logTimestampCheckBox.Location = new System.Drawing.Point(26, 65);
            this.logTimestampCheckBox.Name = "logTimestampCheckBox";
            this.logTimestampCheckBox.Size = new System.Drawing.Size(94, 17);
            this.logTimestampCheckBox.TabIndex = 9;
            this.logTimestampCheckBox.Text = "Log timestamp";
            this.logTimestampCheckBox.UseVisualStyleBackColor = true;
            // 
            // clearButton
            // 
            this.clearButton.Location = new System.Drawing.Point(522, 65);
            this.clearButton.Name = "clearButton";
            this.clearButton.Size = new System.Drawing.Size(75, 23);
            this.clearButton.TabIndex = 8;
            this.clearButton.Text = "Clear Text";
            this.clearButton.UseVisualStyleBackColor = true;
            this.clearButton.Click += new System.EventHandler(this.clearButton_Click);
            // 
            // logTextBox
            // 
            this.logTextBox.Location = new System.Drawing.Point(25, 94);
            this.logTextBox.Multiline = true;
            this.logTextBox.Name = "logTextBox";
            this.logTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.logTextBox.Size = new System.Drawing.Size(572, 187);
            this.logTextBox.TabIndex = 7;
            // 
            // logFileNameTextBox
            // 
            this.logFileNameTextBox.Location = new System.Drawing.Point(76, 24);
            this.logFileNameTextBox.Name = "logFileNameTextBox";
            this.logFileNameTextBox.ReadOnly = true;
            this.logFileNameTextBox.Size = new System.Drawing.Size(334, 20);
            this.logFileNameTextBox.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 28);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Log File:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(639, 559);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "FoxconnTofModuleDemo";
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
        private System.Windows.Forms.CheckBox logTimestampCheckBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox cmdsComboBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox continuousMeasurementCheckBox;
    }
}

