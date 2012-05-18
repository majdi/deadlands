using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;

using wrapper;

namespace deadlands
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        public Window1()
        {
            InitializeComponent();

            _driver = new CliWrapperDriver("deadlands.sys", "deadlands", "\\\\.\\DeadlandsDKOM");
            _filename = "toto";

            this.InitListProcess();
            ListProcesses1.MouseDoubleClick += new MouseButtonEventHandler(ListProcesses1_Selected);
        }

        #region Accessors 

        public string GetFilename
        {
            get { return _filename; }
        }

        #endregion

        #region Driver Operations 

        private void buttonBrowse_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            
            dlg.DefaultExt = ".sys";
            dlg.Filter = "driver|*.sys|all file|*.*"; // Filter files by extension

            Nullable<bool> result = dlg.ShowDialog();
            if (result == true)
            {
                _filename = dlg.FileName;
                MessageBox.Show(dlg.FileName);
            }
        }

        private void buttonRegister_Click(object sender, RoutedEventArgs e)
        {

        }

        private void buttonUnregister_Click(object sender, RoutedEventArgs e)
        {

        }

        private void buttonStart_Click(object sender, RoutedEventArgs e)
        {

        }

        private void buttonStop_Click(object sender, RoutedEventArgs e)
        {

        }

        #endregion

        #region Initialization

        void InitListProcess()
        {
            Process[] processesList = Process.GetProcesses();

            ListProcesses1.Items.Clear();
            foreach (Process proc in processesList)
            {
                ListProcesses1.Items.Add(new { Name = proc.ProcessName, Pid = proc.Id });
            }
        }

        #endregion

        #region Hide Processes with [ DKOM | SSDT ]

        private void ListProcesses1_Selected(object sender, MouseButtonEventArgs e)
        {
            if (ListProcesses1.SelectedItems.Count != 0)
            {
                if (ListProcesses1.SelectedItems != null)
                {
                    foreach (object o in ListProcesses1.SelectedItems)
                    {
                        char[] separtator = { '{', '=', ',', '}' };
                        string[] str = o.ToString().Split(separtator);
                        string process = str[2].Trim();
                        string pid = str[4].Trim();
                        string message = "Do you want to hide the process " + process + " [ pid #" + pid + "]";
                        MessageBoxResult res = MessageBox.Show(message, "Let's Rock!", MessageBoxButton.YesNo);
                        if (res == MessageBoxResult.Yes)
                        {
                            int ret = _driver.DKOMHideProcess(pid);//MessageBox.Show("OK"); //_driver.DriverHideProcess(pid);
                            if (ret == 0)
                            {
                                this.InitListProcess();
                                ListProcesses2.Items.Add(new { Name = process, Pid = pid });
                            }
                            break;
                        }
                        else
                            MessageBox.Show("Can't hide this process");
                    }
                }
            }
        }

        #endregion

        #region Interrupt Descriptor Table

        private void buttonInterrupt_Click(object sender, RoutedEventArgs e)
        {

            if (ListInterrupt.SelectionBoxItem.ToString() == "")
                MessageBox.Show("Choose an interruption :p");
            else
            {
                string message = "Do you want to hook " + ListInterrupt.SelectionBoxItem;
                MessageBoxResult res = MessageBox.Show(message, "Let's Rock!", MessageBoxButton.YesNo);
                if (res == MessageBoxResult.Yes)
                {
                    //int ret = _driver.HookIDT(pid);
                    //if (ret == 0)
                    //{
                    //    //this.InitListProcess();
                    //    //ListProcesses2.Items.Add(new { Name = process, Pid = pid });
                    //}
                    //break;
                }
                else
                    MessageBox.Show("Can't hook this interrupt");
            }
        }

        #endregion

        #region Blue Screen Of Death

        private void buttonBsod_Click(object sender, RoutedEventArgs e)
        {
            MessageBoxResult res = MessageBox.Show("Sure to fuck Redmond?", "Let's Rock!", MessageBoxButton.YesNo);
            if (res == MessageBoxResult.Yes)
                ;
            else
                MessageBox.Show("Can't hook this interrupt");
        }

        #endregion

        #region Attributes 

        private CliWrapperDriver    _driver;
        private string              _filename;

        #endregion

        private void bsod_ImageFailed(object sender, ExceptionRoutedEventArgs e)
        {

        }

    }
}
