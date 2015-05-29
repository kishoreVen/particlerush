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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AssetFolderGeneratorForm));
            this.AssetListLabel = new System.Windows.Forms.Label();
            this.AssetListSpliter = new System.Windows.Forms.SplitContainer();
            this.AssetCategories = new System.Windows.Forms.ListBox();
            this.AssetNames = new System.Windows.Forms.ListBox();
            this.MiscellaneousContainer = new System.Windows.Forms.SplitContainer();
            this.PasswordLabel = new System.Windows.Forms.Label();
            this.UsernameLabel = new System.Windows.Forms.Label();
            this.SyncBtn = new System.Windows.Forms.Button();
            this.GitUrlLabel = new System.Windows.Forms.Label();
            this.GitPassword = new System.Windows.Forms.TextBox();
            this.GitUsername = new System.Windows.Forms.TextBox();
            this.GitCredentialsLabel = new System.Windows.Forms.Label();
            this.AssetLabel = new System.Windows.Forms.Label();
            this.CategoryLabel = new System.Windows.Forms.Label();
            this.AssetName = new System.Windows.Forms.TextBox();
            this.GenerateBtn = new System.Windows.Forms.Button();
            this.AssetCategory = new System.Windows.Forms.TextBox();
            this.AssetGenerationLabel = new System.Windows.Forms.Label();
            this.ControlsLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.AssetListSpliter)).BeginInit();
            this.AssetListSpliter.Panel1.SuspendLayout();
            this.AssetListSpliter.Panel2.SuspendLayout();
            this.AssetListSpliter.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.MiscellaneousContainer)).BeginInit();
            this.MiscellaneousContainer.Panel1.SuspendLayout();
            this.MiscellaneousContainer.Panel2.SuspendLayout();
            this.MiscellaneousContainer.SuspendLayout();
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
            // AssetListSpliter
            // 
            this.AssetListSpliter.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.AssetListSpliter.Location = new System.Drawing.Point(15, 40);
            this.AssetListSpliter.Name = "AssetListSpliter";
            // 
            // AssetListSpliter.Panel1
            // 
            this.AssetListSpliter.Panel1.Controls.Add(this.AssetCategories);
            // 
            // AssetListSpliter.Panel2
            // 
            this.AssetListSpliter.Panel2.Controls.Add(this.AssetNames);
            this.AssetListSpliter.Size = new System.Drawing.Size(590, 300);
            this.AssetListSpliter.SplitterDistance = 295;
            this.AssetListSpliter.TabIndex = 2;
            // 
            // AssetCategories
            // 
            this.AssetCategories.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.AssetCategories.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.AssetCategories.FormattingEnabled = true;
            this.AssetCategories.ItemHeight = 15;
            this.AssetCategories.Location = new System.Drawing.Point(3, 4);
            this.AssetCategories.Name = "AssetCategories";
            this.AssetCategories.Size = new System.Drawing.Size(287, 300);
            this.AssetCategories.TabIndex = 0;
            this.AssetCategories.SelectedIndexChanged += new System.EventHandler(this.AssetCategories_SelectedIndexChanged);
            // 
            // AssetNames
            // 
            this.AssetNames.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.AssetNames.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.AssetNames.FormattingEnabled = true;
            this.AssetNames.ItemHeight = 15;
            this.AssetNames.Location = new System.Drawing.Point(3, 5);
            this.AssetNames.Name = "AssetNames";
            this.AssetNames.Size = new System.Drawing.Size(283, 285);
            this.AssetNames.TabIndex = 0;
            this.AssetNames.SelectedIndexChanged += new System.EventHandler(this.AssetNames_SelectedIndexChanged);
            this.AssetNames.KeyDown += new System.Windows.Forms.KeyEventHandler(this.AssetNames_KeyDown);
            // 
            // MiscellaneousContainer
            // 
            this.MiscellaneousContainer.Location = new System.Drawing.Point(15, 351);
            this.MiscellaneousContainer.Name = "MiscellaneousContainer";
            // 
            // MiscellaneousContainer.Panel1
            // 
            this.MiscellaneousContainer.Panel1.Controls.Add(this.PasswordLabel);
            this.MiscellaneousContainer.Panel1.Controls.Add(this.UsernameLabel);
            this.MiscellaneousContainer.Panel1.Controls.Add(this.SyncBtn);
            this.MiscellaneousContainer.Panel1.Controls.Add(this.GitUrlLabel);
            this.MiscellaneousContainer.Panel1.Controls.Add(this.GitPassword);
            this.MiscellaneousContainer.Panel1.Controls.Add(this.GitUsername);
            this.MiscellaneousContainer.Panel1.Controls.Add(this.GitCredentialsLabel);
            // 
            // MiscellaneousContainer.Panel2
            // 
            this.MiscellaneousContainer.Panel2.Controls.Add(this.AssetLabel);
            this.MiscellaneousContainer.Panel2.Controls.Add(this.CategoryLabel);
            this.MiscellaneousContainer.Panel2.Controls.Add(this.AssetName);
            this.MiscellaneousContainer.Panel2.Controls.Add(this.GenerateBtn);
            this.MiscellaneousContainer.Panel2.Controls.Add(this.AssetCategory);
            this.MiscellaneousContainer.Panel2.Controls.Add(this.AssetGenerationLabel);
            this.MiscellaneousContainer.Size = new System.Drawing.Size(590, 100);
            this.MiscellaneousContainer.SplitterDistance = 295;
            this.MiscellaneousContainer.TabIndex = 8;
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
            this.SyncBtn.Location = new System.Drawing.Point(216, 38);
            this.SyncBtn.Name = "SyncBtn";
            this.SyncBtn.Size = new System.Drawing.Size(75, 53);
            this.SyncBtn.TabIndex = 4;
            this.SyncBtn.Text = "Sync";
            this.SyncBtn.UseVisualStyleBackColor = true;
            this.SyncBtn.Click += new System.EventHandler(this.SyncBtn_Click);
            // 
            // GitUrlLabel
            // 
            this.GitUrlLabel.Location = new System.Drawing.Point(100, 7);
            this.GitUrlLabel.Name = "GitUrlLabel";
            this.GitUrlLabel.Size = new System.Drawing.Size(185, 15);
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
            this.GenerateBtn.Location = new System.Drawing.Point(207, 39);
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
            // ControlsLabel
            // 
            this.ControlsLabel.AutoSize = true;
            this.ControlsLabel.Location = new System.Drawing.Point(89, 15);
            this.ControlsLabel.Name = "ControlsLabel";
            this.ControlsLabel.Size = new System.Drawing.Size(379, 15);
            this.ControlsLabel.TabIndex = 9;
            this.ControlsLabel.Text = "(Q - Show Art Folder, W - Show Unreal Folder, Del / R - Delete Asset)";
            // 
            // AssetFolderGeneratorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.ClientSize = new System.Drawing.Size(624, 459);
            this.Controls.Add(this.ControlsLabel);
            this.Controls.Add(this.MiscellaneousContainer);
            this.Controls.Add(this.AssetListSpliter);
            this.Controls.Add(this.AssetListLabel);
            this.Font = new System.Drawing.Font("Calibri", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "AssetFolderGeneratorForm";
            this.Text = "Asset Folder Generator";
            this.Load += new System.EventHandler(this.AssetFolderGeneratorForm_Load);
            this.AssetListSpliter.Panel1.ResumeLayout(false);
            this.AssetListSpliter.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.AssetListSpliter)).EndInit();
            this.AssetListSpliter.ResumeLayout(false);
            this.MiscellaneousContainer.Panel1.ResumeLayout(false);
            this.MiscellaneousContainer.Panel1.PerformLayout();
            this.MiscellaneousContainer.Panel2.ResumeLayout(false);
            this.MiscellaneousContainer.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.MiscellaneousContainer)).EndInit();
            this.MiscellaneousContainer.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label AssetListLabel;
        private System.Windows.Forms.SplitContainer AssetListSpliter;
        private System.Windows.Forms.ListBox AssetCategories;
        private System.Windows.Forms.ListBox AssetNames;
        private System.Windows.Forms.SplitContainer MiscellaneousContainer;
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
    }
}

