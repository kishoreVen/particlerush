namespace AssetFolderGenerator
{
    partial class AssetFolderGeneratorForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AssetFolderGeneratorForm));
            this.AssetListLabel = new System.Windows.Forms.Label();
            this.AssetCategories = new System.Windows.Forms.ListBox();
            this.AssetNames = new System.Windows.Forms.ListBox();
            this.AssetLabel = new System.Windows.Forms.Label();
            this.CategoryLabel = new System.Windows.Forms.Label();
            this.AssetName = new System.Windows.Forms.TextBox();
            this.GenerateBtn = new System.Windows.Forms.Button();
            this.AssetCategory = new System.Windows.Forms.TextBox();
            this.AssetGenerationLabel = new System.Windows.Forms.Label();
            this.PasswordLabel = new System.Windows.Forms.Label();
            this.UsernameLabel = new System.Windows.Forms.Label();
            this.SyncBtn = new System.Windows.Forms.Button();
            this.GitUrlLabel = new System.Windows.Forms.Label();
            this.GitPassword = new System.Windows.Forms.TextBox();
            this.GitUsername = new System.Windows.Forms.TextBox();
            this.GitCredentialsLabel = new System.Windows.Forms.Label();
            this.ControlsLabel = new System.Windows.Forms.Label();
            this.SelectionPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.AssetTypes = new System.Windows.Forms.ListBox();
            this.MiscOperationsPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.GitCredentialsPanel = new System.Windows.Forms.Panel();
            this.AssetGenerationPanel = new System.Windows.Forms.Panel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.AssetGeneratedName = new System.Windows.Forms.Button();
            this.AssetSuffix = new System.Windows.Forms.MaskedTextBox();
            this.AssetNumber = new System.Windows.Forms.NumericUpDown();
            this.NumberLabel = new System.Windows.Forms.Label();
            this.NameSuffix = new System.Windows.Forms.Label();
            this.NameGenerationLabel = new System.Windows.Forms.Label();
            this.NameGenerationToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SelectionPanel.SuspendLayout();
            this.MiscOperationsPanel.SuspendLayout();
            this.GitCredentialsPanel.SuspendLayout();
            this.AssetGenerationPanel.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.AssetNumber)).BeginInit();
            this.SuspendLayout();
            // 
            // AssetListLabel
            // 
            this.AssetListLabel.AutoSize = true;
            this.AssetListLabel.Font = new System.Drawing.Font("Calibri", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.AssetListLabel.Location = new System.Drawing.Point(13, 13);
            this.AssetListLabel.Name = "AssetListLabel";
            this.AssetListLabel.Size = new System.Drawing.Size(80, 19);
            this.AssetListLabel.TabIndex = 1;
            this.AssetListLabel.Text = "Asset List: ";
            // 
            // AssetCategories
            // 
            this.AssetCategories.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.AssetCategories.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.AssetCategories.FormattingEnabled = true;
            this.AssetCategories.ItemHeight = 15;
            this.AssetCategories.Location = new System.Drawing.Point(10, 10);
            this.AssetCategories.Margin = new System.Windows.Forms.Padding(5);
            this.AssetCategories.Name = "AssetCategories";
            this.AssetCategories.Size = new System.Drawing.Size(283, 285);
            this.AssetCategories.TabIndex = 0;
            this.AssetCategories.SelectedIndexChanged += new System.EventHandler(this.AssetCategories_SelectedIndexChanged);
            // 
            // AssetNames
            // 
            this.AssetNames.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.AssetNames.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.AssetNames.FormattingEnabled = true;
            this.AssetNames.ItemHeight = 15;
            this.AssetNames.Location = new System.Drawing.Point(303, 10);
            this.AssetNames.Margin = new System.Windows.Forms.Padding(5);
            this.AssetNames.Name = "AssetNames";
            this.AssetNames.Size = new System.Drawing.Size(283, 285);
            this.AssetNames.TabIndex = 0;
            this.AssetNames.SelectedIndexChanged += new System.EventHandler(this.AssetNames_SelectedIndexChanged);
            this.AssetNames.KeyDown += new System.Windows.Forms.KeyEventHandler(this.AssetNames_KeyDown);
            // 
            // AssetLabel
            // 
            this.AssetLabel.AutoSize = true;
            this.AssetLabel.Location = new System.Drawing.Point(6, 71);
            this.AssetLabel.Name = "AssetLabel";
            this.AssetLabel.Size = new System.Drawing.Size(41, 15);
            this.AssetLabel.TabIndex = 5;
            this.AssetLabel.Text = "Asset:";
            // 
            // CategoryLabel
            // 
            this.CategoryLabel.AutoSize = true;
            this.CategoryLabel.Location = new System.Drawing.Point(4, 42);
            this.CategoryLabel.Name = "CategoryLabel";
            this.CategoryLabel.Size = new System.Drawing.Size(60, 15);
            this.CategoryLabel.TabIndex = 4;
            this.CategoryLabel.Text = "Category:";
            // 
            // AssetName
            // 
            this.AssetName.Location = new System.Drawing.Point(67, 68);
            this.AssetName.Name = "AssetName";
            this.AssetName.Size = new System.Drawing.Size(130, 23);
            this.AssetName.TabIndex = 3;
            // 
            // GenerateBtn
            // 
            this.GenerateBtn.Location = new System.Drawing.Point(201, 38);
            this.GenerateBtn.Name = "GenerateBtn";
            this.GenerateBtn.Size = new System.Drawing.Size(80, 57);
            this.GenerateBtn.TabIndex = 2;
            this.GenerateBtn.Text = "Asset Generation";
            this.GenerateBtn.UseVisualStyleBackColor = true;
            this.GenerateBtn.Click += new System.EventHandler(this.GenerateBtn_Click);
            // 
            // AssetCategory
            // 
            this.AssetCategory.Location = new System.Drawing.Point(67, 39);
            this.AssetCategory.Name = "AssetCategory";
            this.AssetCategory.Size = new System.Drawing.Size(130, 23);
            this.AssetCategory.TabIndex = 1;
            // 
            // AssetGenerationLabel
            // 
            this.AssetGenerationLabel.AutoSize = true;
            this.AssetGenerationLabel.Location = new System.Drawing.Point(8, 7);
            this.AssetGenerationLabel.Name = "AssetGenerationLabel";
            this.AssetGenerationLabel.Size = new System.Drawing.Size(106, 15);
            this.AssetGenerationLabel.TabIndex = 0;
            this.AssetGenerationLabel.Text = "Asset Generation:";
            // 
            // PasswordLabel
            // 
            this.PasswordLabel.AutoSize = true;
            this.PasswordLabel.Location = new System.Drawing.Point(4, 71);
            this.PasswordLabel.Name = "PasswordLabel";
            this.PasswordLabel.Size = new System.Drawing.Size(34, 15);
            this.PasswordLabel.TabIndex = 6;
            this.PasswordLabel.Text = "Pass:";
            // 
            // UsernameLabel
            // 
            this.UsernameLabel.AutoSize = true;
            this.UsernameLabel.Location = new System.Drawing.Point(4, 44);
            this.UsernameLabel.Name = "UsernameLabel";
            this.UsernameLabel.Size = new System.Drawing.Size(21, 15);
            this.UsernameLabel.TabIndex = 5;
            this.UsernameLabel.Text = "Id:";
            // 
            // SyncBtn
            // 
            this.SyncBtn.Location = new System.Drawing.Point(213, 38);
            this.SyncBtn.Name = "SyncBtn";
            this.SyncBtn.Size = new System.Drawing.Size(63, 53);
            this.SyncBtn.TabIndex = 4;
            this.SyncBtn.Text = "Sync";
            this.SyncBtn.UseVisualStyleBackColor = true;
            this.SyncBtn.Click += new System.EventHandler(this.SyncBtn_Click);
            // 
            // GitUrlLabel
            // 
            this.GitUrlLabel.Location = new System.Drawing.Point(100, 7);
            this.GitUrlLabel.Name = "GitUrlLabel";
            this.GitUrlLabel.Size = new System.Drawing.Size(176, 15);
            this.GitUrlLabel.TabIndex = 3;
            // 
            // GitPassword
            // 
            this.GitPassword.Location = new System.Drawing.Point(47, 68);
            this.GitPassword.Name = "GitPassword";
            this.GitPassword.Size = new System.Drawing.Size(163, 23);
            this.GitPassword.TabIndex = 2;
            this.GitPassword.UseSystemPasswordChar = true;
            // 
            // GitUsername
            // 
            this.GitUsername.Location = new System.Drawing.Point(47, 39);
            this.GitUsername.Name = "GitUsername";
            this.GitUsername.Size = new System.Drawing.Size(163, 23);
            this.GitUsername.TabIndex = 1;
            // 
            // GitCredentialsLabel
            // 
            this.GitCredentialsLabel.AutoSize = true;
            this.GitCredentialsLabel.Location = new System.Drawing.Point(8, 7);
            this.GitCredentialsLabel.Name = "GitCredentialsLabel";
            this.GitCredentialsLabel.Size = new System.Drawing.Size(92, 15);
            this.GitCredentialsLabel.TabIndex = 0;
            this.GitCredentialsLabel.Text = "Git Credentials:";
            // 
            // ControlsLabel
            // 
            this.ControlsLabel.AutoSize = true;
            this.ControlsLabel.Location = new System.Drawing.Point(89, 15);
            this.ControlsLabel.Name = "ControlsLabel";
            this.ControlsLabel.Size = new System.Drawing.Size(379, 15);
            this.ControlsLabel.TabIndex = 9;
            this.ControlsLabel.Text = "(Q - Show Art Folder, W - Show Unreal Folder, Del / R - Delete Asset)";
            // 
            // SelectionPanel
            // 
            this.SelectionPanel.AutoScroll = true;
            this.SelectionPanel.AutoScrollMinSize = new System.Drawing.Size(640, 250);
            this.SelectionPanel.AutoSize = true;
            this.SelectionPanel.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.SelectionPanel.Controls.Add(this.AssetCategories);
            this.SelectionPanel.Controls.Add(this.AssetNames);
            this.SelectionPanel.Controls.Add(this.AssetTypes);
            this.SelectionPanel.Cursor = System.Windows.Forms.Cursors.Hand;
            this.SelectionPanel.Location = new System.Drawing.Point(15, 40);
            this.SelectionPanel.Margin = new System.Windows.Forms.Padding(5);
            this.SelectionPanel.Name = "SelectionPanel";
            this.SelectionPanel.Padding = new System.Windows.Forms.Padding(5);
            this.SelectionPanel.Size = new System.Drawing.Size(887, 305);
            this.SelectionPanel.TabIndex = 10;
            // 
            // AssetTypes
            // 
            this.AssetTypes.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.AssetTypes.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.AssetTypes.FormattingEnabled = true;
            this.AssetTypes.ItemHeight = 15;
            this.AssetTypes.Location = new System.Drawing.Point(594, 8);
            this.AssetTypes.Name = "AssetTypes";
            this.AssetTypes.Size = new System.Drawing.Size(283, 285);
            this.AssetTypes.TabIndex = 1;
            this.AssetTypes.SelectedIndexChanged += new System.EventHandler(this.AssetTypes_SelectedIndexChanged);
            this.AssetTypes.KeyDown += new System.Windows.Forms.KeyEventHandler(this.AssetTypes_KeyDown);
            // 
            // MiscOperationsPanel
            // 
            this.MiscOperationsPanel.AutoScroll = true;
            this.MiscOperationsPanel.AutoScrollMinSize = new System.Drawing.Size(640, 85);
            this.MiscOperationsPanel.AutoSize = true;
            this.MiscOperationsPanel.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.MiscOperationsPanel.Controls.Add(this.GitCredentialsPanel);
            this.MiscOperationsPanel.Controls.Add(this.AssetGenerationPanel);
            this.MiscOperationsPanel.Controls.Add(this.panel1);
            this.MiscOperationsPanel.Location = new System.Drawing.Point(15, 350);
            this.MiscOperationsPanel.Margin = new System.Windows.Forms.Padding(5);
            this.MiscOperationsPanel.Name = "MiscOperationsPanel";
            this.MiscOperationsPanel.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.MiscOperationsPanel.Size = new System.Drawing.Size(887, 107);
            this.MiscOperationsPanel.TabIndex = 11;
            // 
            // GitCredentialsPanel
            // 
            this.GitCredentialsPanel.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.GitCredentialsPanel.Controls.Add(this.PasswordLabel);
            this.GitCredentialsPanel.Controls.Add(this.UsernameLabel);
            this.GitCredentialsPanel.Controls.Add(this.SyncBtn);
            this.GitCredentialsPanel.Controls.Add(this.GitUrlLabel);
            this.GitCredentialsPanel.Controls.Add(this.GitPassword);
            this.GitCredentialsPanel.Controls.Add(this.GitUsername);
            this.GitCredentialsPanel.Controls.Add(this.GitCredentialsLabel);
            this.GitCredentialsPanel.Location = new System.Drawing.Point(8, 3);
            this.GitCredentialsPanel.Name = "GitCredentialsPanel";
            this.GitCredentialsPanel.Size = new System.Drawing.Size(285, 100);
            this.GitCredentialsPanel.TabIndex = 0;
            // 
            // AssetGenerationPanel
            // 
            this.AssetGenerationPanel.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.AssetGenerationPanel.Controls.Add(this.AssetLabel);
            this.AssetGenerationPanel.Controls.Add(this.CategoryLabel);
            this.AssetGenerationPanel.Controls.Add(this.AssetName);
            this.AssetGenerationPanel.Controls.Add(this.GenerateBtn);
            this.AssetGenerationPanel.Controls.Add(this.AssetCategory);
            this.AssetGenerationPanel.Controls.Add(this.AssetGenerationLabel);
            this.AssetGenerationPanel.Location = new System.Drawing.Point(299, 3);
            this.AssetGenerationPanel.Name = "AssetGenerationPanel";
            this.AssetGenerationPanel.Size = new System.Drawing.Size(285, 100);
            this.AssetGenerationPanel.TabIndex = 1;
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.panel1.Controls.Add(this.AssetGeneratedName);
            this.panel1.Controls.Add(this.AssetSuffix);
            this.panel1.Controls.Add(this.AssetNumber);
            this.panel1.Controls.Add(this.NumberLabel);
            this.panel1.Controls.Add(this.NameSuffix);
            this.panel1.Controls.Add(this.NameGenerationLabel);
            this.panel1.Location = new System.Drawing.Point(590, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(285, 100);
            this.panel1.TabIndex = 2;
            // 
            // AssetGeneratedName
            // 
            this.AssetGeneratedName.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.AssetGeneratedName.Enabled = false;
            this.AssetGeneratedName.Location = new System.Drawing.Point(10, 67);
            this.AssetGeneratedName.Name = "AssetGeneratedName";
            this.AssetGeneratedName.Size = new System.Drawing.Size(266, 23);
            this.AssetGeneratedName.TabIndex = 6;
            this.AssetGeneratedName.UseVisualStyleBackColor = false;
            this.AssetGeneratedName.Click += new System.EventHandler(this.AssetGeneratedName_Click);
            // 
            // AssetSuffix
            // 
            this.AssetSuffix.AsciiOnly = true;
            this.AssetSuffix.Location = new System.Drawing.Point(50, 36);
            this.AssetSuffix.Name = "AssetSuffix";
            this.AssetSuffix.Size = new System.Drawing.Size(100, 23);
            this.AssetSuffix.TabIndex = 5;
            this.AssetSuffix.TextChanged += new System.EventHandler(this.AssetSuffix_TextChanged);
            this.AssetSuffix.KeyDown += new System.Windows.Forms.KeyEventHandler(this.AssetSuffix_KeyDown);
            // 
            // AssetNumber
            // 
            this.AssetNumber.Location = new System.Drawing.Point(219, 38);
            this.AssetNumber.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.AssetNumber.Name = "AssetNumber";
            this.AssetNumber.Size = new System.Drawing.Size(57, 23);
            this.AssetNumber.TabIndex = 3;
            this.AssetNumber.ValueChanged += new System.EventHandler(this.AssetNumber_ValueChanged);
            // 
            // NumberLabel
            // 
            this.NumberLabel.AutoSize = true;
            this.NumberLabel.Location = new System.Drawing.Point(158, 42);
            this.NumberLabel.Name = "NumberLabel";
            this.NumberLabel.Size = new System.Drawing.Size(57, 15);
            this.NumberLabel.TabIndex = 2;
            this.NumberLabel.Text = "Number:";
            // 
            // NameSuffix
            // 
            this.NameSuffix.AutoSize = true;
            this.NameSuffix.Location = new System.Drawing.Point(7, 42);
            this.NameSuffix.Name = "NameSuffix";
            this.NameSuffix.Size = new System.Drawing.Size(41, 15);
            this.NameSuffix.TabIndex = 1;
            this.NameSuffix.Text = "Suffix:";
            // 
            // NameGenerationLabel
            // 
            this.NameGenerationLabel.AutoSize = true;
            this.NameGenerationLabel.Location = new System.Drawing.Point(7, 7);
            this.NameGenerationLabel.Name = "NameGenerationLabel";
            this.NameGenerationLabel.Size = new System.Drawing.Size(109, 15);
            this.NameGenerationLabel.TabIndex = 0;
            this.NameGenerationLabel.Text = "Name Generation:";
            // 
            // NameGenerationToolTip
            // 
            this.NameGenerationToolTip.IsBalloon = true;
            this.NameGenerationToolTip.ToolTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            this.NameGenerationToolTip.ToolTipTitle = "Name Generation";
            // 
            // AssetFolderGeneratorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.ClientSize = new System.Drawing.Size(910, 459);
            this.Controls.Add(this.MiscOperationsPanel);
            this.Controls.Add(this.SelectionPanel);
            this.Controls.Add(this.ControlsLabel);
            this.Controls.Add(this.AssetListLabel);
            this.Font = new System.Drawing.Font("Calibri", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "AssetFolderGeneratorForm";
            this.Text = "Asset Folder Generator";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.AssetFolderGeneratorForm_FormClosed);
            this.Load += new System.EventHandler(this.AssetFolderGeneratorForm_Load);
            this.SelectionPanel.ResumeLayout(false);
            this.MiscOperationsPanel.ResumeLayout(false);
            this.GitCredentialsPanel.ResumeLayout(false);
            this.GitCredentialsPanel.PerformLayout();
            this.AssetGenerationPanel.ResumeLayout(false);
            this.AssetGenerationPanel.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.AssetNumber)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label AssetListLabel;        
        private System.Windows.Forms.ListBox AssetCategories;
        private System.Windows.Forms.ListBox AssetNames;
        private System.Windows.Forms.TextBox GitPassword;
        private System.Windows.Forms.TextBox GitUsername;
        private System.Windows.Forms.Label GitCredentialsLabel;
        private System.Windows.Forms.Label GitUrlLabel;
        private System.Windows.Forms.Button SyncBtn;
        private System.Windows.Forms.Button GenerateBtn;
        private System.Windows.Forms.TextBox AssetCategory;
        private System.Windows.Forms.Label AssetGenerationLabel;
        private System.Windows.Forms.TextBox AssetName;
        private System.Windows.Forms.Label PasswordLabel;
        private System.Windows.Forms.Label UsernameLabel;
        private System.Windows.Forms.Label AssetLabel;
        private System.Windows.Forms.Label CategoryLabel;
        private System.Windows.Forms.Label ControlsLabel;
        private System.Windows.Forms.FlowLayoutPanel SelectionPanel;
        private System.Windows.Forms.ListBox AssetTypes;
        private System.Windows.Forms.FlowLayoutPanel MiscOperationsPanel;
        private System.Windows.Forms.Panel GitCredentialsPanel;
        private System.Windows.Forms.Panel AssetGenerationPanel;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label NameGenerationLabel;
        private System.Windows.Forms.Label NumberLabel;
        private System.Windows.Forms.Label NameSuffix;
        private System.Windows.Forms.NumericUpDown AssetNumber;
        private System.Windows.Forms.MaskedTextBox AssetSuffix;
        private System.Windows.Forms.ToolTip NameGenerationToolTip;
        private System.Windows.Forms.Button AssetGeneratedName;
    }
}

