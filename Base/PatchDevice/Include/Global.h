#ifndef GlobalH
#define GlobalH

#include <MusicTools.h>

#ifndef NON_DAISY_DEVICE

#include <sys/_stdint.h>

#else // NON_DAISY_DEVICE

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#endif // QT_NO_DEBUG_STREAM

#endif // NON_DAISY_DEVICE

#endif // GlobalH
