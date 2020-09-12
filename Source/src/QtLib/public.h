#ifndef PUBLIC_H
#define PUBLIC_H

#define SAFE_DELETE(p) if (nullptr!=p) {\
                            delete p;\
                            p = nullptr;\
                        }

#endif // PUBLIC_H
