﻿#region License Information (GPL v3)

/*
    ShareX - A program that allows you to take screenshots and share any file type
    Copyright (c) 2007-2017 ShareX Team

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

    Optionally you can also view the license at <http://www.gnu.org/licenses/>.
*/

#endregion License Information (GPL v3)

using System.Drawing;
using System.Drawing.Drawing2D;

namespace ShareX.ScreenCaptureLib
{
    public class ImageScreenDrawingShape : BaseDrawingShape
    {
        public override ShapeType ShapeType { get; } = ShapeType.DrawingImageScreen;

        public Image Image { get; private set; }

        public override void OnDraw(Graphics g)
        {
            if (Image == null)
            {
                if (IsValidShape)
                {
                    Rectangle rect = Manager.LimitRectangleToImage(Rectangle);
                    Manager.DrawRegionArea(g, rect, true);
                }
            }
            else
            {
                DrawImage(g);
            }
        }

        protected void DrawImage(Graphics g)
        {
            if (Image != null)
            {
                if (Manager.IsRenderingOutput)
                {
                    g.InterpolationMode = InterpolationMode.HighQualityBicubic;
                }
                else
                {
                    g.InterpolationMode = InterpolationMode.NearestNeighbor;
                }

                g.DrawImage(Image, Rectangle);

                g.InterpolationMode = InterpolationMode.Bilinear;
            }
        }

        public override void OnCreated()
        {
            if (IsValidShape)
            {
                Rectangle = Manager.LimitRectangleToImage(Rectangle);
                Image = Manager.CropImage(Rectangle);
            }

            if (Image == null)
            {
                Remove();
            }
        }

        public override void Dispose()
        {
            if (Image != null)
            {
                Image.Dispose();
            }
        }
    }
}