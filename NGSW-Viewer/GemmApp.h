// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// Define a new application type, each program should derive a class from wxApp
class GemmApp : public wxApp
{
public:
    // This method is called on application startup and is a good place for the app
    // initialization (doing it here and not in the constructor allows to have an 
    // error return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

