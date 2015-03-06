using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

using Microsoft.VisualBasic;
using System.Xml;

namespace FileStructureGenerator
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        #region Subsystem Variables
        private SourceControlManager    mSourceControlManager;
        private AssetManager            mAssetManager;
        private FileManager             mFileManager;
        #endregion

        #region Private Selection Variables
        private string currentlySelectedAssetName;
        private string currentlySelectedAssetPrefix;
        private string currentlyGeneratedAssetFileName;
        #endregion

        #region Constructor
        public MainWindow()
        {
            InitializeComponent();

            InitializeSubsystems();
            
            /* Local Textbox and Textblock initializations */
            currentlyGeneratedAssetFileName     = null;
            currentlySelectedAssetName          = null;
            currentlySelectedAssetPrefix        = null;

            SelectedAssetName.Text          = Globals.finalAssetNameLabel + "None";
            GeneratedAssetPrefix.Text       = Globals.finalAssetPrefixLabel + "None";
            FinalGeneratedName.Text         = Globals.finalAssetFileLabel + "None";


            /* First Run Setup */
            if (ConfigurationManager.IsFirstRun)
            {
                UndoSettings.IsEnabled = false;

                Btn_SwitchPage.IsEnabled = false;

                ShowPage(PageEnum.SettingsPage);
            }
            else
            {
                mSourceControlManager.AuthenticateRepository();

                ShowPage(PageEnum.MainPage);
            }            
        }

        public void InitializeSubsystems()
        {
            /* Initialize Git Manager */
            mSourceControlManager = new SourceControlManager();

            
            /* Initialize Asset Manager */
            mAssetManager = new AssetManager();

            /* Initialize File Manager */
            mFileManager = new FileManager();
        }
        #endregion

        #region Methods

        #region Root Director Helpers
        private void CheckMismatchesBetweenAssetListAndWorkingDirectory()
        {
            string assetsFolder = Globals.RepositoryRoot + Globals.gameAssetsFolderName;
            string[] directories = Directory.GetDirectories(assetsFolder);
            string[] directoryNames = new string[directories.Length];
            
            for(int index = 0; index < directories.Length; index ++ )
            {
                DirectoryInfo directoryInfo = new DirectoryInfo(directories[index]);
                directoryNames[index] = directoryInfo.Name;
            }

            string[] mismatchList = mAssetManager.GetMismatchesFromList(directoryNames);

            foreach(string mismatch in mismatchList)
            {
                ClearAllCreatedDirectories(mismatch, true);
            }

            mSourceControlManager.PushChangesToRepository();
        }
        #endregion

        #region Directory Structure
        private bool CreateDirectoryStructure(string assetName)
        {
            bool success = false;

            success = CreateArtDirectoryStructure(assetName);

            if(success == true)
            {
                success = CreateUnrealImportDirectoryStructure(assetName);
            }            

            return success;
        }

        private bool CreateArtDirectoryStructure(string assetName)
        {
            bool directoryCreationSuccessful = false;

            string assetFolder = Globals.RepositoryRoot + Globals.gameAssetsFolderName + assetName;            

            string conceptFolder        = assetFolder + "\\Concept";
            string assetExportFolder    = assetFolder + "\\AssetExport";
            string texturesFolder       = assetExportFolder + "\\Textures";

            try
            {
                // Create Asset Root Directory
                Directory.CreateDirectory(assetFolder);
                File.WriteAllText(assetFolder + "\\Readme.txt", assetName + " -- Main Asset Folder");

                // Child Directories
                Directory.CreateDirectory(conceptFolder);
                File.WriteAllText(conceptFolder + "\\Readme.txt", assetName + " -- Conpcet Folder");
                Directory.CreateDirectory(assetExportFolder);
                File.WriteAllText(assetExportFolder + "\\Readme.txt", assetName + " -- Main Asset Export Folder");

                // Child Sub Directories

                Directory.CreateDirectory(texturesFolder);
                File.WriteAllText(texturesFolder + "\\Readme.txt", assetName + " -- Textures Folder For Artist");                

                MessageBoxResult confirmation = System.Windows.MessageBox.Show("The Structure has been successfully created at " + assetFolder + ". Do you wish to open the folder in explorer?", "Structure Creation Successful", MessageBoxButton.YesNo, MessageBoxImage.Information);

                if (confirmation == MessageBoxResult.Yes)
                {
                    directoryCreationSuccessful = true;

                    Process.Start("explorer.exe", assetFolder);
                }

                #region Directory Commit
                string folderRelToRoot = Globals.gameAssetsFolderName + assetName;

                mSourceControlManager.StageDirectoryForCommit(folderRelToRoot);
                #endregion

                directoryCreationSuccessful = true;
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.Message, "Structure Creation Error", MessageBoxButton.OK, MessageBoxImage.Error);

                directoryCreationSuccessful = false;
            }

            return directoryCreationSuccessful;
        }

        private bool CreateUnrealImportDirectoryStructure(string assetName)
        {
            bool directoryCreationSuccessful = false;

            string assetImportFolder = Globals.RepositoryRoot + Globals.unrealWorkFolder + Globals.unrealImportFolder;

            directoryCreationSuccessful = CreateFoldersInImport(assetImportFolder, Globals.unrealImportTestFolder, assetName);

            directoryCreationSuccessful = CreateFoldersInImport(assetImportFolder, Globals.unrealImportFinalFolder, assetName);

            return directoryCreationSuccessful;
        }

        private bool CreateFoldersInImport(string assetImportFolder, string relFolder, string assetName)
        {
            bool directoryCreationSuccessful = false;

            string assetFolder = assetImportFolder + relFolder + assetName;

            try
            {
                // Create Asset Root Directory
                Directory.CreateDirectory(assetFolder);

                // Create ReadMe structure for model
                string readMeText = assetFolder + "\\ReadMe.txt";

                File.WriteAllText(readMeText, "## Asset Name: " + assetName + " ##");

                // Create Directory for Animation fbx
                string animDirectory = assetFolder + "\\Anims";

                Directory.CreateDirectory(animDirectory);

                // Establish Temp file for folder commit
                string animTempText = animDirectory + "\\AnimDetails.txt";

                File.WriteAllText(animTempText, "## Anim No. - Animation Name - Frame Start, Frame End ##");

                string folderRelToRoot = Globals.unrealWorkFolder + Globals.unrealImportFolder + relFolder + assetName;

                mSourceControlManager.StageDirectoryForCommit(folderRelToRoot);

                directoryCreationSuccessful = true;
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.Message, "Structure Creation Error", MessageBoxButton.OK, MessageBoxImage.Error);

                directoryCreationSuccessful = false;
            }

            return directoryCreationSuccessful;
        }

        private void ClearAllCreatedDirectories(string assetName, bool isDelete = false)
        {
            string assetFolder = Globals.RepositoryRoot + Globals.gameAssetsFolderName + assetName;

            string assetImportFolder = Globals.RepositoryRoot + Globals.unrealWorkFolder + Globals.unrealImportFolder;
            string assetImportTestFolder = assetImportFolder + Globals.unrealImportTestFolder + assetName;
            string assetImportFinalFolder = assetImportFolder + Globals.unrealImportFinalFolder + assetName;

            if(Directory.Exists(assetFolder))
            {
                if(!isDelete)
                {
                    mSourceControlManager.UnstageDirectory(assetFolder);
                    Directory.Delete(assetFolder, true);
                }
                else
                {
                    Directory.Delete(assetFolder, true);
                    mSourceControlManager.StageDirectoryForCommit(assetFolder, "Deleting Asset Folders: " + assetName);
                }
            }

            if (Directory.Exists(assetImportTestFolder))
            {
                if (!isDelete)
                {
                    mSourceControlManager.UnstageDirectory(assetImportTestFolder);
                    Directory.Delete(assetImportTestFolder, true);
                }
                else
                {
                    Directory.Delete(assetImportTestFolder, true);
                    mSourceControlManager.StageDirectoryForCommit(assetImportTestFolder, "Deleting Import Test Folders: " + assetName);
                }
            }

            if (Directory.Exists(assetImportFinalFolder))
            {
                if (!isDelete)
                {
                    mSourceControlManager.UnstageDirectory(assetImportFinalFolder);
                    Directory.Delete(assetImportFinalFolder, true);
                }
                else
                {
                    Directory.Delete(assetImportFinalFolder, true);
                    mSourceControlManager.StageDirectoryForCommit(assetImportFinalFolder, "Deleting Import Final Folders: " + assetName);
                }
            }
        }
        #endregion

        #region Name Generator Methods
        private void GenerateFinalAssetName()
        {
            string assetFileName = Globals.finalAssetFileLabel;

            currentlyGeneratedAssetFileName = string.Empty;

            if(currentlySelectedAssetName != null && currentlySelectedAssetPrefix != null)
            {
                currentlyGeneratedAssetFileName += currentlySelectedAssetPrefix + "_" + currentlySelectedAssetName;

                if(GeneratedNameModifier.Text.Length > 0)
                {
                    currentlyGeneratedAssetFileName +=  "_" + GeneratedNameModifier.Text;
                }

                assetFileName += currentlyGeneratedAssetFileName;
            }
            else
            {
                currentlyGeneratedAssetFileName = null;

                assetFileName += "None";
            }

            FinalGeneratedName.Text = assetFileName;
        }
        #endregion

        #region Switch Selections
        private void ShowPage(PageEnum page)
        {
            ResetAllPages();

            switch (page)
            {
                case PageEnum.MainPage:
                    MainPage.Visibility = System.Windows.Visibility.Visible;

                    mSourceControlManager.SyncRepository();
                    CheckMismatchesBetweenAssetListAndWorkingDirectory();

                    PopulateAssetTree();

                    PopulateAssetTypeTree();

                    GenerateFinalAssetName();

                    Btn_SwitchPage.Content = Globals.btnToSettingsPage;

                    if(!ConfigurationManager.IsAdmin())
                    {
                        AddAssetType.Visibility = System.Windows.Visibility.Collapsed;
                    }
                    break;

                case PageEnum.SettingsPage:
                    SettingsPage.Visibility = System.Windows.Visibility.Visible;
                    
                    if(Globals.RepositoryRoot.Length == 0)
                    {
                        FolderSelector.Content = Globals.rootNotSelected;
                    }
                    else
                    {
                        FolderSelector.Content = Globals.RepositoryRoot;
                    }

                    BitbucketUsername.Text = Globals.BitbucketUsername;
                    BitbucketEmail.Text = Globals.BitbucketEmail;
                    BitbucketPassword.Password = Globals.BitbucketPassword;

                    Btn_SwitchPage.Content = Globals.btnToMainPage;
                    break;                
            }
        }

        private void ResetAllPages()
        {            
            SettingsPage.Visibility = System.Windows.Visibility.Collapsed;
            MainPage.Visibility = System.Windows.Visibility.Collapsed;    
        }
        #endregion

        #region Utilities
        private Tuple<string, string> CamelCaseWord(string completeText)
        {
            string casedWord = "";

            string casedSpacedWord = "";

            string[] assetWordSplit = completeText.Split(' ');

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
        #endregion

        #endregion

        #region Events Callback

        #region Root Director Block
        private void FolderSelector_Click(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog rootPicker = new FolderBrowserDialog();
            rootPicker.Description = "Pick the root directory of repository you cloned from git.";

            while(true)
            {
                DialogResult pickResult = rootPicker.ShowDialog();

                if (pickResult == System.Windows.Forms.DialogResult.OK)
                {
                    if(!ConfigurationManager.CheckParticleRushGitRoot(rootPicker.SelectedPath))
                    {
                        System.Windows.MessageBox.Show("The path you selected is not the root folder of particle rush repository. Please select the correct folder.", "Not particle rush repository", MessageBoxButton.OK, MessageBoxImage.Error);

                        continue;
                    }
                    else
                    {
                        // Update the Folder Picker
                        FolderSelector.Content = rootPicker.SelectedPath;                        

                        break;
                    }                    
                }
                else
                {
                    break;
                }

            }            
        }
        #endregion        

        #endregion

        #region Tree Explorer Methods
        private List<TreeViewItem> TreeBuilder<T>(List<T> data)
        {
            /* Asset Categories Collection */
            List<TreeViewItem> categoryCollection = new List<TreeViewItem>();

            /* Setup Categories */
            TreeViewItem allCategory = new TreeViewItem();            

            if(typeof(T) == typeof(AssetData))
            {
                allCategory.Header = Globals.assetExplorerAllCategory;                
            }
            else if(typeof(T) == typeof(FileData))
            {
                allCategory.Header = Globals.assetTypeExplorerAllCategory;                
            }            


            string currentTag = string.Empty;
            TreeViewItem currentCategory = null;

            foreach (T genricAsset in data)
            {
                TreeViewItem categoryItem = new TreeViewItem();
                TreeViewItem treeItem = new TreeViewItem();

                if(genricAsset is AssetData)
                {
                    AssetData asset = genricAsset as AssetData;

                    if (!asset.assetTag.Equals(currentTag))
                    {
                        if (currentCategory != null)
                        {
                            categoryCollection.Add(currentCategory);
                        }

                        currentCategory = new TreeViewItem();
                        currentCategory.Header = asset.assetTag;
                        currentTag = asset.assetTag;
                    }


                    categoryItem.Header = asset.assetFriendlyName;
                    categoryItem.Uid = asset.assetName;
                    categoryItem.MouseDoubleClick += treeItem_MouseDoubleClick;                    


                    treeItem.Header = asset.assetFriendlyName;
                    treeItem.Uid = asset.assetName;
                    treeItem.MouseDoubleClick += treeItem_MouseDoubleClick;        
                }
                else if(genricAsset is FileData)
                {
                    FileData file = genricAsset as FileData;

                    if (!file.fileTag.Equals(currentTag))
                    {
                        if (currentCategory != null)
                        {
                            categoryCollection.Add(currentCategory);
                        }

                        currentCategory = new TreeViewItem();
                        currentCategory.Header = file.fileTag;
                        currentTag = file.fileTag;
                    }


                    categoryItem.Header = file.fileFriendlyType;
                    categoryItem.Uid = file.fileType;


                    treeItem.Header = file.fileFriendlyType;
                    treeItem.Uid = file.fileType;
                }

                currentCategory.Items.Add(categoryItem);
                allCategory.Items.Add(treeItem);
            }

            if (currentCategory != null)
            {
                categoryCollection.Add(currentCategory);
            }

            categoryCollection.Insert(0, allCategory);

            return categoryCollection;
        }

        private void PopulateAssetTree()
        {
            AssetExplorer.Items.Clear();

            List<AssetData> assetList = mAssetManager.AssetList;

            if(assetList != null)
            {
                assetList.Sort(AssetData.CompareByTags);

                List<TreeViewItem> builtTree = TreeBuilder<AssetData>(assetList);

                foreach (TreeViewItem category in builtTree)
                {
                    AssetExplorer.Items.Add(category);
                }
            }            
        }

        private void PopulateAssetTypeTree()
        {
            AssetTypeExplorer.Items.Clear();

            List<FileData> fileChart = mFileManager.FileChart;

            if(fileChart != null)
            {
                fileChart.Sort(FileData.CompareByTags);

                List<TreeViewItem> builtTree = TreeBuilder<FileData>(fileChart);

                foreach (TreeViewItem category in builtTree)
                {
                    AssetTypeExplorer.Items.Add(category);
                }
            }            
        }
        #endregion

        #region Tree Explorer Event Callback
        private void AssetExplorer_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            TreeViewItem x = AssetExplorer.SelectedItem as TreeViewItem;
            
            if(x != null)
            {
                if (x.HasItems)
                {
                    if (x.IsSelected)
                    {
                        x.IsSelected = false;
                    }

                    currentlySelectedAssetName = null;

                    SelectedAssetName.Text = Globals.finalAssetNameLabel + "None";
                }
                else
                {
                    currentlySelectedAssetName = x.Uid;

                    SelectedAssetName.Text = Globals.finalAssetNameLabel + x.Header;
                }                
            }

            GenerateFinalAssetName();
        }

        private void treeItem_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            TreeViewItem selectedItem = AssetExplorer.SelectedItem as TreeViewItem;

            string assetFolder = Globals.RepositoryRoot + Globals.gameAssetsFolderName + selectedItem.Uid;

            Process.Start("explorer.exe", assetFolder);
        }

        private void AssetTypeExplorer_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            TreeViewItem x = AssetTypeExplorer.SelectedItem as TreeViewItem;

            if(x != null)
            {
                if (x.HasItems)
                {
                    if (x.IsSelected)
                    {
                        x.IsSelected = false;
                    }

                    currentlySelectedAssetPrefix = null;

                    GeneratedAssetPrefix.Text = Globals.finalAssetPrefixLabel + "None";
                }
                else
                {
                    string assetTypeName = x.Uid;

                    FileData fileData = mFileManager.GetFileType(assetTypeName);

                    currentlySelectedAssetPrefix = fileData.filePrefix;

                    GeneratedAssetPrefix.Text = Globals.finalAssetPrefixLabel + fileData.fileFriendlyType;
                }
            }            

            GenerateFinalAssetName();
        }
        #endregion        

        #region Button Callabcks
        private void Btn_AddAsset_Click(object sender, RoutedEventArgs e)
        {
            if (Globals.RepositoryRoot == Globals.rootNotSelected)
            {
                System.Windows.MessageBox.Show("You need to choose the root directory of your project's git repository to create a directory structure.", "Repository Root Not Selected", MessageBoxButton.OK, MessageBoxImage.Error);

                return;
            }

            string[] assetInfo = Txt_AddAsset.Text.Split(';');

            if(assetInfo.Length != 2)
            {
                System.Windows.MessageBox.Show("To add a new asset you need to follow the (" + Globals.assetNamePlaceHolder + ") format.", "Asset Addition Format Error", MessageBoxButton.OK, MessageBoxImage.Error);

                Txt_AddAsset.Text = Globals.assetNamePlaceHolder;

                return;
            }

            string assetName = assetInfo[0];
            string assetTag = assetInfo[1].Trim();

            if (assetName != Globals.assetNamePlaceHolder || assetName.Length != 0)
            {
                Tuple<string, string> modifiedAssetWorlds = CamelCaseWord(assetName);

                string finalAssetName = modifiedAssetWorlds.Item1;
                string finalAssetFriendlyName = modifiedAssetWorlds.Item2;

                if (finalAssetName.Length > 0)
                {
                    if (CreateDirectoryStructure(finalAssetName))
                    {
                        if (mAssetManager.AddAsset(finalAssetName, finalAssetFriendlyName, assetTag))
                        {
                            /* Commit folder as well */

                            /* Do cleanup on Successfully added */
                            mSourceControlManager.StageFileForCommit(Globals.gameAssetListFileName, "Added new Asset Type: " + finalAssetFriendlyName);
                            mSourceControlManager.PushChangesToRepository();

                            PopulateAssetTree();
                        }
                    }
                    else
                    {
                        ClearAllCreatedDirectories(finalAssetName);
                    }
                }
            }

            Txt_AddAsset.Text = Globals.assetNamePlaceHolder;
        }

        private void Btn_AddAssetType_Click(object sender, RoutedEventArgs e)
        {
            if (Globals.RepositoryRoot == Globals.rootNotSelected)
            {
                System.Windows.MessageBox.Show("You need to choose the root directory of your project's git repository to create a directory structure.", "Repository Root Not Selected", MessageBoxButton.OK, MessageBoxImage.Error);

                return;
            }

            string[] assetInfo = Txt_AddAssetType.Text.Split(';');

            if (assetInfo.Length != 3)
            {
                System.Windows.MessageBox.Show("To add a new asset you need to follow the (" + Globals.assetTypePlaceHolder + ") format.", "Asset Addition Format Error", MessageBoxButton.OK, MessageBoxImage.Error);

                Txt_AddAssetType.Text = Globals.assetNamePlaceHolder;

                return;
            }

            string typeName = assetInfo[0];
            string typePrexix = assetInfo[1].Trim();
            string typeTag = assetInfo[2].Trim();

            if (typeName != Globals.assetTypePlaceHolder || typeName.Length != 0)
            {
                Tuple<string, string> modifiedAssetWorlds = CamelCaseWord(typeName);

                string finalTypeName = modifiedAssetWorlds.Item1;
                string finalFriendlyTypeName = modifiedAssetWorlds.Item2;

                if (finalTypeName.Length > 0)
                {
                    if (mFileManager.AddNewFileType(finalTypeName, finalFriendlyTypeName, typePrexix, typeTag))
                    {
                        mSourceControlManager.StageFileForCommit(Globals.fileChartLocation, "Updated File Chart");
                        mSourceControlManager.PushChangesToRepository();

                       PopulateAssetTypeTree();                       
                    }
                }
            }

            Txt_AddAssetType.Text = Globals.assetTypePlaceHolder;
        }

        private void CopyToClipboard_Click(object sender, RoutedEventArgs e)
        {
            if (currentlyGeneratedAssetFileName != null)
            {
                System.Windows.Clipboard.SetText(currentlyGeneratedAssetFileName);
            }
        }

        private void SaveSettings_Click(object sender, RoutedEventArgs e)
        {
            string repositoryRootFromDialog = FolderSelector.Content as string;

            if(BitbucketUsername.Text.Length != 0 && BitbucketEmail.Text.Length != 0 && BitbucketPassword.Password.Length != 0 && repositoryRootFromDialog != Globals.rootNotSelected)
            {
                if (!ConfigurationManager.CheckParticleRushGitRoot(repositoryRootFromDialog))
                {
                    System.Windows.MessageBox.Show("The path you selected in the repository root is no longer valid as the root folder of particle rush repository. Please select the correct folder.", "Not particle rush repository", MessageBoxButton.OK, MessageBoxImage.Error);

                    return;
                }

                Globals.RepositoryRoot      = repositoryRootFromDialog;
                Globals.BitbucketUsername   = BitbucketUsername.Text;
                Globals.BitbucketEmail      = BitbucketEmail.Text;
                Globals.BitbucketPassword   = BitbucketPassword.Password;

                if(ConfigurationManager.IsFirstRun)
                {
                    UndoSettings.IsEnabled = true;

                    Btn_SwitchPage.IsEnabled = true;

                    ConfigurationManager.IsFirstRun = false;
                }

                mSourceControlManager.AuthenticateRepository();

                ShowPage(PageEnum.MainPage);
            }            
        }

        private void UndoSettings_Click(object sender, RoutedEventArgs e)
        {
            FolderSelector.Content = Globals.RepositoryRoot;

            BitbucketUsername.Text  = Globals.BitbucketUsername;
            BitbucketEmail.Text     = Globals.BitbucketEmail;
            BitbucketPassword.Password = Globals.BitbucketPassword;

            ShowPage(PageEnum.MainPage);
        }

        private void Btn_SwitchPage_Click(object sender, RoutedEventArgs e)
        {
            if((Btn_SwitchPage.Content as string) == Globals.btnToMainPage)
            {
                ShowPage(PageEnum.MainPage);                
            }
            else if((Btn_SwitchPage.Content as string) == Globals.btnToSettingsPage)
            {
                ShowPage(PageEnum.SettingsPage);
            }
        }
        #endregion        

        #region Textbox Callbacks
        private void GeneratedNameModifier_GotFocus(object sender, RoutedEventArgs e)
        {
            if(GeneratedNameModifier.Text == Globals.finalModifierPlaceholder)
            {
                GeneratedNameModifier.Text = "";
            }
        }

        private void GeneratedNameModifier_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (GeneratedNameModifier.Text != Globals.finalModifierPlaceholder)
            {
                GenerateFinalAssetName();
            }
        }

        private void Txt_AddAsset_GotFocus(object sender, RoutedEventArgs e)
        {
            if (Txt_AddAsset.Text == Globals.assetNamePlaceHolder)
            {
                Txt_AddAsset.Text = "; ";
            }
        }

        private void Txt_AddAssetType_GotFocus(object sender, RoutedEventArgs e)
        {
            if (Txt_AddAssetType.Text == Globals.assetTypePlaceHolder)
            {
                Txt_AddAssetType.Text = "; ; ";
            }
        }
        #endregion        
    }
}
