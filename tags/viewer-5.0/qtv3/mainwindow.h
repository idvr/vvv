// (c) by Stefan Roettger, licensed under GPL 2+

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef HAVE_QT5
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include "volren_qgl.h"
#include "prefwindow.h"
#include "swipeSlider.h"

class QTV3VolRenWidget: public QGLVolRenWidget
{
   Q_OBJECT;

public:

   QTV3VolRenWidget(QWidget * parent = 0, bool stereo = false)
      : QGLVolRenWidget(parent, stereo)
   {timer_.start();}

protected:

   virtual void updating()
   {
      emit updating_signal();
   }

   virtual void update(const char *info,float percent)
   {
      QString text;

      if (percent>0.0f)
         text=QString("%1: %2%").arg(info).arg((int)(100.0f*percent+0.5f));
      else
         text=QString("%1").arg(info);

      emit update_signal(text);

      if (timer_.elapsed()>1000.0f/fps_)
      {
         repaint();
         QApplication::processEvents();
         timer_.restart();
      }
   }

   virtual void updated()
   {
      emit updated_signal();
   }

   virtual void updated_rotation()
   {
      emit interaction_signal();
   }

   virtual void updated_orientation()
   {
      emit orientation_signal();
   }

   virtual void updated_zoom()
   {
      emit interaction_signal();
   }

   virtual void updated_clipping()
   {
      emit interaction_signal();
   }

   virtual void updated_opacity()
   {
      emit interaction_signal();
   }

   virtual void updated_measuring(double px,double py,double pz,double length,double endlength)
   {
      emit measuring_signal(px,py,pz,length,endlength);
   }

   QTime timer_;

signals:

   void updating_signal();
   void update_signal(QString text);
   void updated_signal();

   void orientation_signal();
   void interaction_signal();
   void measuring_signal(double px,double py,double pz,double length,double endlength);
};

class QTV3Slider: public QSlider
{
public:

   QTV3Slider(Qt::Orientation orientation, QWidget * parent = 0)
      : QSlider(orientation, parent)
   {o_=orientation;}

   //! return preferred minimum window size
   QSize minimumSizeHint() const
   {
      if (o_==Qt::Horizontal)
         return(QSize(150, 50));
      else
         return(QSize(50, 100));
   }

protected:

   Qt::Orientation o_;
};

class QTV3MainWindow: public QMainWindow
{
   Q_OBJECT; // Qt Metacall object for signal/slot connections

public:

   //! default ctor
   QTV3MainWindow(QWidget *parent = 0,
                  bool stereo = false,
                  bool demo = false,
                  bool touch = false);

   //! dtor
   ~QTV3MainWindow();

   //! load a volume
   void loadVolume(const char *filename);

   //! load a DICOM series
   void loadSeries(const std::vector<std::string> list);

   //! load a surface
   void loadSurface(const char *filename);

   //! extract an iso surface
   void extractIsoSurface(float isovalue);

   //! set volume rotation
   void setAngle(double angle);

   //! set volume rotation speed
   void setRotation(double omega);

   //! set volume tilt
   void setTilt(double tilt=0.0);

   //! set volume rotation angle
   void setTiltXY(double tiltXY=0.0);

   //! set volume rotation angle
   void setTiltYZ(double tiltYZ=0.0);

   //! set volume tilt animation
   void setAnimatedTilt(double tilt=0.0,double omega=60);

   //! set volume clipping
   void setClip(double clip);

   //! set volume tilt
   void setZoom(double zoom);

   //! set zoom animation
   void setAnimatedZoom(double zoom=0.0,double freq=1.0/60);

   //! clear volume
   void clearVolume();

   //! clear surface
   void clearSurface();

   //! use linear transfer function
   void setTF(float center=0.5f,float size=1.0f,
              bool inverse=FALSE);

   //! set global emission
   void setEmission(float emi);

   //! get global emission
   float getEmission();

   //! set global absorption
   void setAbsorption(float att);

   //! get global absorption
   float getAbsorption();

   //! set color hue
   void setColorHue(float hue);

   //! set gradmag mode
   void setGradMag();

   //! set oversampling
   void setOversampling();

   //! set undersampling
   void setUndersampling();

   //! set anaglyph stereo mode
   void setAnaglyph();

   //! set interlaced stereo mode
   void setSFXmode(int sfxmode=0);

   //! set stereo parameters
   void setSFXparams(float sfxbase=1.0f,float sfxfocus=1.0f);

   //! set maximum idle time before reset
   void setMaxIdle(double t);

   //! set maximum volume size
   void volMaxSizeChange(unsigned int vol_maxsize);

   //! set maximum iso size
   void isoMaxSizeChange(unsigned int iso_maxsize);

   //! grab window
   void grab();

private:

   QMenu *fileMenu_;
   QMenu *helpMenu_;

   QVBoxLayout *mainLayout_;
   QSplitter *mainSplitter_;
   QSplitter *viewerSplitter_;

   QVBoxLayout *viewerLayout_;
   QHBoxLayout *sliderLayout_;

   QTV3VolRenWidget *vrw_;
   bool vrw_stereo_;

   bool demo_;
   bool touch_;

   QLabel *update_;

   void createMenus();
   void createWidgets();
   void createDocks();

   void reset(const char *teaser="Drop.pvm", const char *path="/usr/share/qtv3/");

   std::string getPrefix(const std::vector<std::string> list);

   QStringList browse(QString path="",
                      bool newfile=false);

   QTV3Slider *createSlider(int minimum, int maximum, int value,
                            bool vertical=false);

   SwipeSlider *createSwipeSlider(int minimum, int maximum, int value,
                                  bool vertical=false, QString text="");

   bool hasTeaserVolume_;

   int flipXY1_,flipXY2_;
   int flipYZ1_,flipYZ2_;

   void checkTilt();

   QTV3Slider *clipSlider_;
   SwipeSlider *clipDemoSlider_;
   int clipNum_;

   QTV3Slider *zoomSlider_;
   SwipeSlider *zoomDemoSlider_;
   QTV3Slider *rotSlider_;
   QTV3Slider *tiltSlider_;
   QTV3Slider *emiSlider_;
   QTV3Slider *attSlider_;

   QCheckBox *gradMagCheck_;
   QCheckBox *invModeCheck_;
   QCheckBox *rotateCheck_;
   QCheckBox *reverseCheck_;
   QCheckBox *planeCheck_;
   QCheckBox *showIsoCheck_;
   QCheckBox *clipIsoCheck_;

   QRadioButton *sfxOffCheck_;
   QRadioButton *anaModeCheck_;
   QRadioButton *sfxModeCheck_;
   QRadioButton *sfxOnCheck_;

   QCheckBox *flipCheckXY1_;
   QCheckBox *flipCheckXY2_;
   QCheckBox *flipCheckYZ1_;
   QCheckBox *flipCheckYZ2_;

   QRadioButton *sampleButton1_;
   QRadioButton *sampleButton2_;
   QRadioButton *sampleButton3_;

   QPushButton *modeButton1_;
   QPushButton *modeButton2_;
   QPushButton *modeButton3_;
   QPushButton *modeButton4_;
   QPushButton *modeButton5_;
   QPushButton *modeButton6_;
   QPushButton *modeButton7_;
   QPushButton *modeButton8_;

   QPushButton *grabButton_;
   QPushButton *resetButton_;

   QTV3PrefWindow *prefs_;

protected:

   bool idling_;
   QTime last_event_;
   QTimer idle_check_;
   double max_idle_time_;

   // monitor time of last event
   bool eventFilter(QObject *target, QEvent *event)
   {
      bool userevent = false;

      if (dynamic_cast<QMouseEvent*>(event))
         if (event->type() != QEvent::MouseMove)
            userevent = true;

      if (dynamic_cast<QKeyEvent*>(event))
         userevent = true;

      if (userevent)
      {
         last_event_.start();
         idling_ = false;
      }

      return QMainWindow::eventFilter(target, event);
   }

   // get idle time in seconds since last event
   double idle()
      {return(last_event_.elapsed()/1000.0);}

   void keyPressEvent(QKeyEvent *event)
   {
      if (event->key() == Qt::Key_Q)
         emit close();
      else if (event->key() == Qt::Key_I)
         extractIsoSurface();

      QMainWindow::keyPressEvent(event);
   }

   void keyReleaseEvent(QKeyEvent *event)
   {
      QMainWindow::keyReleaseEvent(event);
   }

   void dragEnterEvent(QDragEnterEvent *event);
   void dragMoveEvent(QDragMoveEvent *event);
   void dragLeaveEvent(QDragLeaveEvent *event);

public:

   void dropEvent(QDropEvent *event);

protected:

   static QString normalizeFile(QString file);

protected slots:

   void open();
   void zoom(int v);
   void zoomDemo(double v);
   void rotate(int v);
   void tilt(int v);
   void clip(int v);
   void clipDemo(double v);
   void tack();
   void clear();
   void emission(int v);
   void absorption(int v);
   void checkGradMag(int on);
   void checkInvMode(int on);
   void checkRotate(int on);
   void checkReverse(int on);
   void checkPlane(int on);
   void checkShowIso(int on);
   void checkClipIso(int on);
   void checkSFXoff(bool on);
   void checkAnaMode(bool on);
   void checkSFXMode(bool on);
   void checkSFXon(bool on);
   void checkFlipXY1(int on);
   void checkFlipXY2(int on);
   void checkFlipYZ1(int on);
   void checkFlipYZ2(int on);
   void samplingChanged1(bool on);
   void samplingChanged2(bool on);
   void samplingChanged3(bool on);
   void modeChanged1(bool on);
   void modeChanged2(bool on);
   void modeChanged3(bool on);
   void modeChanged4(bool on);
   void modeChanged5(bool on);
   void modeChanged6(bool on);
   void modeChanged7(bool on);
   void modeChanged8(bool on);
   void grabWindow();
   void disableRotation();
   void resetInteractions();
   void resetDefaults();
   void extractIsoSurface();
   void clearIsoSurface();
   void prefs();
   void about();

   void updating_slot();
   void update_slot(QString text);
   void updated_slot();

   void orientation_slot();
   void interaction_slot();
   void measuring_slot(double px,double py,double pz,double length,double endlength);

   void idle_check();

private:

   double default_omega_;
   double default_angle_;
   double default_tilt_;
   double default_tiltXY_;
   double default_tiltYZ_;
   double default_animtilt_;
   double default_animomega_;
   double default_clip_;
   double default_zoom_;
   double default_animzoom_;
   double default_animfreq_;
   double default_tfcenter_;
   double default_tfsize_;
   bool default_tfinverse_;
   double default_tfemi_;
   double default_tfatt_;
   double default_hue_;
   bool default_gradmag_;
   bool default_anaglyph_;
   int default_sfxmode_;
   float default_sfxbase_;
   float default_sfxfocus_;

   void checkSFX(bool stereo);
};

#endif
