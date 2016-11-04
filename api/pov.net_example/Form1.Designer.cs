namespace povray.test
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
            this.components = new System.ComponentModel.Container();
            this.renderResult = new System.Windows.Forms.PictureBox();
            this.button1 = new System.Windows.Forms.Button();
            this.renderProgress = new System.Windows.Forms.ProgressBar();
            this.renderUpdate = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.renderResult)).BeginInit();
            this.SuspendLayout();
            // 
            // renderResult
            // 
            this.renderResult.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.renderResult.Location = new System.Drawing.Point(12, 54);
            this.renderResult.Name = "renderResult";
            this.renderResult.Size = new System.Drawing.Size(912, 585);
            this.renderResult.TabIndex = 0;
            this.renderResult.TabStop = false;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "Render";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // renderProgress
            // 
            this.renderProgress.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.renderProgress.Location = new System.Drawing.Point(12, 645);
            this.renderProgress.Name = "renderProgress";
            this.renderProgress.Size = new System.Drawing.Size(912, 23);
            this.renderProgress.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.renderProgress.TabIndex = 2;
            // 
            // renderUpdate
            // 
            this.renderUpdate.Tick += new System.EventHandler(this.renderUpdate_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(936, 680);
            this.Controls.Add(this.renderProgress);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.renderResult);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.renderResult)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox renderResult;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ProgressBar renderProgress;
        private System.Windows.Forms.Timer renderUpdate;
    }
}

