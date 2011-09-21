#ifndef MIMXAPPLICATION_H
#define MIMXAPPLICATION_H

#include "mimapplication.h"

#include "mpassthruwindow.h"
#include "mimremotewindow.h"
#include "mimxextension.h"

class MImXApplication : public MIMApplication
{
    Q_OBJECT
public:
    MImXApplication(int &argc, char** argv);
    virtual ~MImXApplication();

    static MImXApplication* instance();

    const MImXCompositeExtension& compositeExtension() { return mCompositeExtension; }
    const MImXDamageExtension& damageExtension() { return mDamageExtension; }

    //! Requires a valid remoteWinId and a valid passThruWindow before it'll
    //! start to work.
    //! \sa setTransientHint(), setPassThruWindow();
    bool x11EventFilter(XEvent *ev);

    virtual void setTransientHint(WId remoteWinId);

    virtual bool selfComposited() const;
    bool manualRedirection() const;
    bool bypassWMHint() const;

    //! Flag that is used to sync between MAbstractInputMethodHost and
    //! rotation animation in order to capture the VKB without
    //! self-composited background drawn.
    void setSuppressBackground(bool suppress);

    virtual QWidget *toplevel() const;
    QWidget *passThruWindow() const;
    virtual QWidget *pluginsProxyWidget() const;

    virtual const QPixmap &remoteWindowPixmap();

#ifdef UNIT_TEST
    MImRemoteWindow *remoteWindow() const;
#endif

Q_SIGNALS:
    //! This signal is emitted when remote window is changed.
    //! Parameter can be 0 if window is unmapped.
    void remoteWindowChanged(MImRemoteWindow *newWindow);

private Q_SLOTS:
    void updatePassThruWindow(const QRegion &region);

private:
    virtual void finalize();

    void parseArguments(int &argc, char** argv);

    void handleTransientEvents(XEvent *ev);
    void handleRemoteWindowEvents(XEvent *ev);
    void handlePassThruMapEvent(XEvent *ev);

    MImXCompositeExtension mCompositeExtension;
    MImXDamageExtension mDamageExtension;

    bool mSelfComposited;
    bool mManualRedirection;
    bool mBypassWMHint;
    bool mBackgroundSuppressed;

    std::auto_ptr<MPassThruWindow> mPassThruWindow;
    std::auto_ptr<QWidget> mPluginsProxyWidget;
    std::auto_ptr<MImRemoteWindow> mRemoteWindow;

    friend class Ut_PassthroughServer;
};

#endif // MIMXAPPLICATION_H