using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using ShareX.ScreenCaptureLib;
using System.Drawing;
namespace ScreenCapture
{
    static class Program
    {
        static int Main(string[] Args)
        {
            if (Args.Length != 2)
                return 0;

            try
            {
                Image image = null;
                switch (Args[0])
                {
                    case "-region":
                        {
                            RegionCaptureForm form = new RegionCaptureForm(RegionCaptureMode.Default);
                            form.Prepare();
                            form.ShowDialog();
                            image = form.GetResultImage();
                        }
                        break;
                }

                image.Save(Args[1]);
            }
            catch
            {
                return 0;
            }

            return 1;
        }
    }
}
