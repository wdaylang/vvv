// (c) by Stefan Roettger, licensed under GPL 2+

#ifdef HAVE_QT5
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#endif

#include "volren_qgl.h"
#include "mainwindow.h"

double get_opt(QString o)
{
   o=o.mid(o.indexOf("=")+1);
   return(o.toDouble());
}

void usage(const char *prog)
{
   QString name(prog);
   std::cout << "usage:" << std::endl;
   std::cout << " " << name.mid(name.lastIndexOf("/")+1).toStdString() << " {options} [volume | series]" << std::endl;
   std::cout << "where options are:" << std::endl;
   std::cout << " --demo: demo gui" << std::endl;
   std::cout << " --omega=x: auto-rotation speed (degrees/s)" << std::endl;
   std::cout << " --angle=x: rotation angle (degrees)" << std::endl;
   std::cout << " --tilt=x: tilt angle (degrees)" << std::endl;
   std::cout << " --tiltXY=x: XY rotation angle (degrees)" << std::endl;
   std::cout << " --tiltYZ=x: YZ rotation angle (degrees)" << std::endl;
   std::cout << " --zoom=x: zoom (percent)" << std::endl;
   std::cout << " --fullscreen: use full screen rendering mode" << std::endl;
   std::cout << " --tfcenter=x: center of the linear transfer function window (0-1)" << std::endl;
   std::cout << " --tfsize=x: size of the linear transfer function window (0-1)" << std::endl;
   std::cout << " --tfemi=x: global emission (percent)" << std::endl;
   std::cout << " --tfatt=x: global attenuation (percent)" << std::endl;
   std::cout << " --hue: color hue in degrees (0-360)" << std::endl;
   std::cout << " --gradmag: use gradient magnitude rendering mode" << std::endl;
   std::cout << " --anaglyph: use anaglyph stereo rendering mode" << std::endl;
   std::cout << " --stereo: use left/right stereo buffer rendering mode" << std::endl;
   std::cout << " --isovalue=x: extracted iso surface value (0-1)" << std::endl;
   std::cout << " --maxidle=x: maximum idle time before gui reset is x seconds" << std::endl;
   std::cout << " --maxvol=x: maximum edge size of volume to be loaded (default=512)" << std::endl;
   std::cout << " --maxiso=x: maximum edge size of volume to be extracted (default=256)" << std::endl;
   std::cout << " --help: this help text" << std::endl;
   std::cout << "where volume is:" << std::endl;
   std::cout << " a single .pvm or .rek volume file" << std::endl;
   std::cout << "where series is:" << std::endl;
   std::cout << " a series of DICOM .dcm or .imd image files" << std::endl;
   std::cout << "example:" << std::endl;
   std::cout << " ./qtv3 --demo --fullscreen --zoom=50 --maxidle=60 Bucky.pvm" << std::endl;
   exit(0);
}

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

   if (!QGLFormat::hasOpenGL()) return(1);

   setlocale(LC_NUMERIC, "C");

   // get argument list
   QStringList args = QCoreApplication::arguments();

   // scan for arguments and options
   QStringList arg,opt;
   for (int i=1; i<args.size(); i++)
      if (args[i].startsWith("--")) opt.push_back(args[i].mid(2));
      else if (args[i].startsWith("-")) opt.push_back(args[i].mid(1));
      else arg.push_back(args[i]);

   bool demo=false;
   double omega=30;
   double angle=0;
   double tilt=0;
   double tiltXY=0;
   double tiltYZ=0;
   double zoom=0;
   bool fullscreen=false;
   double tfcenter=0.5;
   double tfsize=1.0;
   double tfemi=100;
   double tfatt=100;
   double hue=360.0;
   bool gradmag=false;
   bool anaglyph=false;
   bool stereo=false;
   double isovalue=0;
   double maxidle=0;
   unsigned int maxvol=0;
   unsigned int maxiso=0;

   // scan option list
   for (int i=0; i<opt.size(); i++)
      if (opt[i]=="demo") demo=true;
      else if (opt[i].startsWith("omega=")) omega=get_opt(opt[i]);
      else if (opt[i].startsWith("angle=")) angle=get_opt(opt[i]);
      else if (opt[i].startsWith("tilt=")) tilt=get_opt(opt[i]);
      else if (opt[i].startsWith("tiltXY=")) tiltXY=get_opt(opt[i]);
      else if (opt[i].startsWith("tiltYZ=")) tiltYZ=get_opt(opt[i]);
      else if (opt[i].startsWith("zoom=")) zoom=get_opt(opt[i]);
      else if (opt[i]=="fullscreen") fullscreen=true;
      else if (opt[i].startsWith("tfcenter=")) tfcenter=get_opt(opt[i]);
      else if (opt[i].startsWith("tfsize=")) tfsize=get_opt(opt[i]);
      else if (opt[i].startsWith("tfemi=")) tfemi=get_opt(opt[i]);
      else if (opt[i].startsWith("tfatt=")) tfatt=get_opt(opt[i]);
      else if (opt[i].startsWith("hue=")) hue=get_opt(opt[i]);
      else if (opt[i]=="gradmag") gradmag=true;
      else if (opt[i]=="anaglyph") anaglyph=true;
      else if (opt[i]=="stereo") stereo=true;
      else if (opt[i].startsWith("isovalue=")) isovalue=get_opt(opt[i]);
      else if (opt[i].startsWith("maxidle=")) maxidle=get_opt(opt[i]);
      else if (opt[i].startsWith("maxvol=")) maxvol=get_opt(opt[i]);
      else if (opt[i].startsWith("maxiso=")) maxiso=get_opt(opt[i]);
      else if (opt[i]=="help") usage(argv[0]);
      else usage(argv[0]);

   QTV3MainWindow main(NULL, stereo, demo);

   if (arg.size()==1)
   {
      QString file=arg[0];

      if (file.endsWith(".geo"))
      {
         main.loadSurface(file.toStdString().c_str());
      }
      else
      {
         main.loadVolume(file.toStdString().c_str());
      }
   }
   else if (arg.size()>1)
   {
      std::vector<std::string> list;
      for (int i=0; i<arg.size(); i++) list.push_back(arg[i].toStdString());
      main.loadSeries(list);
   }

   if (fullscreen) main.showFullScreen();
   else main.show();

   main.setRotation(omega);
   if (omega==0.0) main.setAngle(angle);
   if (tilt!=0.0) main.setTilt(tilt);
   if (tiltXY!=0.0) main.setTiltXY(tiltXY);
   if (tiltYZ!=0.0) main.setTiltYZ(tiltYZ);
   if (zoom!=0.0) main.setZoom(zoom/100.0);
   if (tfcenter!=0.5 || tfsize!=1.0) main.setTF(tfcenter,tfsize);
   if (hue!=360.0) main.setColorHue(hue);
   if (gradmag) main.setGradMag();
   if (anaglyph) main.setAnaglyph();
   if (tfemi!=100.0) main.setEmission(tfemi/100.0*main.getEmission());
   if (tfatt!=100.0) main.setAbsorption(tfatt/100.0*main.getAbsorption());
   if (isovalue!=0.0) main.extractIsoSurface(isovalue);
   if (maxidle>0.0) main.setMaxIdle(maxidle);
   if (maxvol>0) main.volMaxSizeChange(maxvol);
   if (maxiso>0) main.isoMaxSizeChange(maxiso);

   return(app.exec());
}
