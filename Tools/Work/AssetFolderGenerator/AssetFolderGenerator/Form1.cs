using Microsoft.Office.Interop.Word;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AssetFolderGenerator
{
    public partial class AssetFolderGeneratorForm : Form
    {
        #region CONST
        private const string LIST_ALL = "All";
        private static object _oEndOfDoc = "\\endofdoc";
        #endregion

        #region FIELDS
        private ConfigurationManager                                    mConfigurationManager;
        private AssetManager                                            mAssetManager;
        private GitManager                                              mGitManager;
        private AssetTypeManager                                        mAssetTypeManager;
        private int                                                     mPreviousSelectedCategory;
        private int                                                     mPreviousSelectedAsset;
        private int                                                     mPreviousSelectedAssetType;
        #endregion

        #region CONSTRUCTOR
        public AssetFolderGeneratorForm()
        {
            InitializeComponent();

            mConfigurationManager       = new ConfigurationManager();
            mGitManager                 = new GitManager();
            mAssetManager               = new AssetManager(mConfigurationManager, mGitManager);
            mAssetTypeManager           = new AssetTypeManager(mConfigurationManager, mGitManager);

            mPreviousSelectedCategory   = -1;
            mPreviousSelectedAsset      = -1;
            mPreviousSelectedAssetType  = 0;
        }
        #endregion

        #region CALLBACKS
        private void SyncBtn_Click(object sender, EventArgs e)
        {
            string username = GitUsername.Text;
            string password = GitPassword.Text;

            if ("" == username || "" == password)
                return;

            if(username != mConfigurationManager.GitUsername)
                mConfigurationManager.GitUsername = username;
            if(password != mConfigurationManager.GitPassword)
                mConfigurationManager.GitPassword = password;

            Sync(username, password);
        }

        private void GenerateBtn_Click(object sender, EventArgs e)
        {
            string username             = mConfigurationManager.GitUsername;
            string password             = mConfigurationManager.GitPassword;

            string assetCategory        = AssetCategory.Text;
            string assetFriendlyName    = AssetName.Text;

            if ("" == username || "" == password || "" == assetCategory || "" == assetFriendlyName)
                return;

            Tuple<string, string> assetTuple = CamelCaseWord(assetFriendlyName);
            
            Sync(username, password);

            mAssetManager.AddAsset(assetTuple.Item1, assetTuple.Item2, assetCategory);
            mGitManager.PushChangesToRepository(username, password);

            UpdateAssetUI(LIST_ALL);

            AssetName.Text = "";
        }

        private void AssetFolderGeneratorForm_Load(object sender, EventArgs e)
        {
            GitUrlLabel.Text = mConfigurationManager.RepositoryRoot;

            // Sync Repository
            Sync(mConfigurationManager.GitUsername, mConfigurationManager.GitPassword);

            // Set Username and Password
            GitUsername.Text = mConfigurationManager.GitUsername;
            GitPassword.Text = mConfigurationManager.GitPassword;

            // Set to 0, to Select All
            UpdateAssetUI(LIST_ALL);
            UpdateAssetTypeUI();
        }

        private void AssetFolderGeneratorForm_FormClosed(object sender, FormClosedEventArgs e)
        {
        }

        private void AssetCategories_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mPreviousSelectedCategory == AssetCategories.SelectedIndex)
                return;

            string selectedCategory = AssetCategories.Items[AssetCategories.SelectedIndex].ToString();
            mPreviousSelectedCategory = AssetCategories.SelectedIndex;

            if(selectedCategory != LIST_ALL)
                AssetCategory.Text = selectedCategory;

            UpdateAssetUI(selectedCategory);
            BuildAssetName();
        }

        private void AssetNames_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mPreviousSelectedAsset == AssetNames.SelectedIndex)
                return;

            mPreviousSelectedAsset = AssetNames.SelectedIndex;

            BuildAssetName();
        }

        private void AssetTypes_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mPreviousSelectedAssetType == AssetTypes.SelectedIndex || AssetTypes.SelectedIndex == -1)
                return;

            mPreviousSelectedAssetType = AssetTypes.SelectedIndex;

            BuildAssetName();
        }

        private void AssetSuffix_TextChanged(object sender, EventArgs e)
        {
            BuildAssetName();
        }

        private void AssetNumber_ValueChanged(object sender, EventArgs e)
        {
            BuildAssetName();
        }

        private void AssetNames_KeyDown(object sender, KeyEventArgs e)
        {
            if (mPreviousSelectedAsset < 0 || mPreviousSelectedAsset >= AssetNames.Items.Count)
                return;

            string selectedAsset = AssetNames.Items[mPreviousSelectedAsset].ToString();

            if (e.KeyCode == Keys.W)
                mAssetManager.ShowImportAssetInExplorer(selectedAsset);
            else if (e.KeyCode == Keys.Q)
                mAssetManager.ShowArtAssetInExplorer(selectedAsset);
            else if (e.KeyCode == Keys.Delete || e.KeyCode == Keys.R)
                DeleteSelectedAsset(selectedAsset);
        }

        private void AssetTypes_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.N)
            {
                mAssetTypeManager.AddAssetType();
                mPreviousSelectedAssetType = 0;
                UpdateAssetTypeUI();
            }
            else if (e.KeyCode == Keys.Delete || e.KeyCode == Keys.R)
            {
                if (mPreviousSelectedAssetType < 0 || mPreviousSelectedAssetType >= AssetTypes.Items.Count)
                    return;

                string selectedAssetType = AssetTypes.Items[mPreviousSelectedAssetType].ToString();
                mAssetTypeManager.RemoveAssetType(selectedAssetType);
            }
        }

        private void AssetSuffix_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string text = AssetSuffix.Text;

                if("" != text)
                {
                    AssetSuffix.Text = CheckSpelling(text);
                }
            }
        }

        private void AssetGeneratedName_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.Clipboard.SetText(AssetGeneratedName.Text);
        }
        #endregion

        #region METHODS
        private void UpdateAssetUI(string selectedCategory)
        {
            AssetCategories.Items.Clear();
            AssetNames.Items.Clear();

            List<AssetData> assetList   = mAssetManager.AssetList;
            
            List<string> categories     = new List<string>();
            List<string> assets         = new List<string>();
            categories.Add(LIST_ALL);

            foreach(AssetData asset in assetList)
            {
                if(selectedCategory == LIST_ALL || selectedCategory == asset.assetTag)
                    AssetNames.Items.Add(asset.assetName);

                if (!categories.Contains(asset.assetTag))
                    categories.Add(asset.assetTag);
            }

            categories.Sort();

            int index = categories.FindIndex(((x) => selectedCategory == x));
            foreach (string category in categories)
            {
                AssetCategories.Items.Add(category);
            }

            AssetCategories.SetSelected(index, true);

            AssetCategories.Focus();
            AssetNames.Focus();
        }

        private void UpdateAssetTypeUI()
        {
            AssetTypes.Items.Clear();

            List<AssetTypeData> assetTypeList = mAssetTypeManager.AssetTypeList;

            foreach (AssetTypeData assetType in assetTypeList)
            {
                AssetTypes.Items.Add(assetType.assetTypeFriendlyName);
            }

            if(assetTypeList.Count != 0)
            {
                mPreviousSelectedAssetType = System.Math.Max(0, System.Math.Min(mPreviousSelectedAssetType, assetTypeList.Count - 1));
                AssetTypes.SetSelected(mPreviousSelectedAssetType, true);

                AssetTypes.Focus();
            }
        }

        private void Sync(string username, string password)
        {
            if ("" != username && "" != password)
            {
                mGitManager.AuthenticateRepository(username, password, mConfigurationManager.RepositoryRoot);
                mGitManager.SyncRepository(username, password);

                UpdateAssetUI(LIST_ALL);

                UpdateAssetTypeUI();
            }
        }

        private Tuple<string, string> CamelCaseWord(string completeText)
        {
            string      casedWord               = "";
            string      casedSpacedWord         = "";
            string[]    assetWordSplit          = completeText.Split(' ');

            foreach (string assetWord in assetWordSplit)
            {
                if (assetWord.Length > 0)
                {
                    string modifiedAssetWord = assetWord.ToLower();

                    char[] charArrayWord = modifiedAssetWord.ToCharArray();
                    charArrayWord[0] = char.ToUpper(charArrayWord[0]);

                    modifiedAssetWord = new string(charArrayWord);

                    casedWord += modifiedAssetWord;
                    if (casedSpacedWord.Length == 0)
                    {
                        casedSpacedWord += modifiedAssetWord;
                    }
                    else
                    {
                        casedSpacedWord += " " + modifiedAssetWord;
                    }
                }
            }

            return new Tuple<string, string>(casedWord, casedSpacedWord);
        }

        private void DeleteSelectedAsset(string selectedAsset)
        {
            string username = mConfigurationManager.GitUsername;
            string password = mConfigurationManager.GitPassword;

            if ("" == username || "" == password)
                return;

            // Sync Repository
            Sync(mConfigurationManager.GitUsername, mConfigurationManager.GitPassword);

            mAssetManager.RemoveAsset(selectedAsset);
            mGitManager.PushChangesToRepository(username, password);

            //Clamp previous selected category to be 0 or max - 1
            int selectedCategoryIndex = System.Math.Max(0, System.Math.Min(mPreviousSelectedCategory, AssetCategories.Items.Count - 1));
            UpdateAssetUI(AssetCategories.Items[selectedCategoryIndex].ToString());
        }

        private void BuildAssetName()
        {
            string assetname = "";
            bool isComplete = true;

            if(AssetTypes.SelectedIndex != -1)
            {
                string selectedAssetType = AssetTypes.Items[AssetTypes.SelectedIndex].ToString();
                AssetTypeData selectedAsset = mAssetTypeManager.GetAsset(selectedAssetType);

                if(selectedAsset != null)
                    assetname += selectedAsset.assetTypePrefix;
                else
                    isComplete = false;
            }
            else
            {
                if (AssetTypes.Items.Count != 0 && mPreviousSelectedAssetType < AssetTypes.Items.Count)
                {
                    string selectedAssetType = AssetTypes.Items[mPreviousSelectedAssetType].ToString();
                    AssetTypeData selectedAsset = mAssetTypeManager.GetAsset(selectedAssetType);

                    if (selectedAsset != null)
                        assetname += selectedAsset.assetTypePrefix;
                    else
                        isComplete = false;
                }
                else
                    isComplete = false;
            }

            if (AssetNames.SelectedIndex != -1)
                assetname += "_" + AssetNames.Items[AssetNames.SelectedIndex].ToString();
            else
                isComplete = false;

            if (AssetSuffix.Text != "")
                assetname += "_" + AssetSuffix.Text;

            if ((int)AssetNumber.Value != 0)
                assetname += "_" + AssetNumber.Value.ToString();

            AssetGeneratedName.Text = assetname;
            AssetGeneratedName.Enabled = isComplete;
        }

        public string CheckSpelling(string inText)
        {
            int errors = 0;
            Microsoft.Office.Interop.Word.Application mWordApp = new Microsoft.Office.Interop.Word.Application { Visible = false }; ;
            Document mWordDoc = mWordApp.Documents.Add();
            mWordApp.ActiveWindow.SetFocus();

            mWordDoc.Words.First.InsertBefore(inText);
            Microsoft.Office.Interop.Word.ProofreadingErrors spellErrorsColl = mWordDoc.SpellingErrors;
            errors = spellErrorsColl.Count;

            object optional = Missing.Value;

            mWordDoc.CheckSpelling(
                ref optional, ref optional, ref optional, ref optional, ref optional, ref optional,
                ref optional, ref optional, ref optional, ref optional, ref optional, ref optional);

            object first = 0;
            object last = mWordDoc.Characters.Count - 1;
            string outText = mWordDoc.Range(ref first, ref last).Text;

            mWordDoc.Close(WdSaveOptions.wdDoNotSaveChanges, WdOriginalFormat.wdOriginalDocumentFormat, optional);
            mWordApp.Quit();

            return outText;
        }
        #endregion
    }
}