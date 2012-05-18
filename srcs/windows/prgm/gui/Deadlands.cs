/*
**
** M2.Malicious Code - Project: deadlands
** Filename	: [ deadlands.c ]
** Epi-Tek4 : [ Armand Morgan, Hochwelker Florent, Toumi Majdi ]
**
*/

using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows;

namespace deadlands
{
    //[DllImport("Driver.dll", EntryPoint = "HideProcess")]
    //public static extern void HideProcess();

    //[DllImport("libCppForInterop.dll", EntryPoint = "CDriverDelete")]
    //public static extern void DeleteCPersonne(IntPtr cp);

    class Deadlands
    {
        #region Ctor/Dtor 
        
        public Deadlands(string filename, string name)
        {
            _filename = filename;
            _name = name;
        }

        ~Deadlands()
        {
        }

        #endregion

        #region Accessors 

        public string GetFilename
        {
            get { return _filename; }
        }

        public string GetName
        {
            get { return _name; }
        }

        #endregion

        #region Main methods 

        //[System.Runtime.InteropServices.DllImport("kernel32", SetLastError = true)]
        //static extern unsafe System.IntPtr CreateFile
        //(
        //    string FileName,          // file name
        //    uint DesiredAccess,       // access mode
        //    uint ShareMode,           // share mode
        //    uint SecurityAttributes,  // Security Attributes
        //    uint CreationDisposition, // how to create
        //    uint FlagsAndAttributes,  // file attributes
        //    int hTemplateFile         // handle to template file
        //);

        //[DllImport("kernel32.dll", SetLastError = true)]
        //static extern unsafe int WriteFile(IntPtr handle, IntPtr buffer,
        //  int numBytesToWrite, IntPtr numBytesWritten, NativeOverlapped* lpOverlapped);

        //[System.Runtime.InteropServices.DllImport("kernel32", SetLastError = true)]
        //static extern unsafe bool WriteFile
        //(
        //    System.IntPtr hFile,      // handle to file
        //    void* pBuffer,            // data buffer
        //    int NumberOfBytesToRead,  // number of bytes to read
        //    int* pNumberOfBytesRead,  // number of bytes read
        //    int Overlapped            // overlapped buffer
        //);

        //[System.Runtime.InteropServices.DllImport("kernel32", SetLastError = true)]
        //static extern unsafe bool CloseHandle
        //(
        //    System.IntPtr hObject // handle to object
        //);

        //public void  DriverHideProcess(string pid)
        //{
        //    IntPtr      hFile;
        //    string      deviceName;
        //    UInt32      dwReturn;

        //    deviceName = "\\\\.\\DeadlandsDKOM";// +this._name;
        //    MessageBox.Show(deviceName);
        //    hFile = CreateFile(deviceName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
        //    if (hFile == new IntPtr(-1))
        //        MessageBox.Show("Can't create service!");
        //    else
        //    {
        //        MessageBox.Show("YATTA!");
        //        //WriteFile(hFile, pid, pid.Length. )
        //    }
        //    //Write

        //    //HANDLE			hFile;
        //    //DWORD           dwReturn;
        //    //INT32           len;
        //    //char          *name;
            
        //    //len = strlen(args[0]) + strlen("\\\\.\\");
        //    //name = malloc(sizeof(*name) * len);
        //    //strcat(strcpy(name, "\\\\.\\"), args[0]);
            
        //    //WriteFile(hFile, args[1], strlen(args[1]), &dwReturn, NULL);
        //    //CloseHandle(hFile);
        //}

        #endregion

        #region Attributes 

        string _filename;
        string _name;

        //const uint GENERIC_WRITE = 0x40000000;
        //const uint GENERIC_READ = 0x80000000;
        //const uint OPEN_EXISTING = 0x3;
 
        #endregion
    }
}
