/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|						     (c) SuSE Linux AG |
\----------------------------------------------------------------------/

  File:		NCPackageSelectorPluginImpl.cc

  Author:	Hedgehog Painter <kmachalkova@suse.cz>
	        \\\'>

/-*/

#include "NCPackageSelectorPluginImpl.h"
#include "NCPackageSelectorStart.h"

#include <YTableHeader.h>
#include <Y2Log.h>

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPackageSelectorPluginStub::createPackageSelector
//	METHOD TYPE : YWidget
//
//	DESCRIPTION : Create NCPackageSelectorStart which reads the layout
//                    term of the package selection dialog, creates the widget
//		      tree and creates the NCPackageSelector.
//
NCPackageSelectorPluginImpl PSP;

YPackageSelector * NCPackageSelectorPluginImpl::createPackageSelector( YWidget * parent,
								   long modeFlags )
{
    YWidget * w = 0;

    try
    {
	w = new NCPackageSelectorStart ( parent, modeFlags, YD_HORIZ );
    }
    catch (const std::exception & e)
    {
	UIERR << "Caught a std::exception: " << e.what () << endl;
    }
    catch (...)
    {
	UIERR << "Caught an unspecified exception" << endl;
    }

    // FIXME - remove debug logging
    YDialog::currentDialog()->dumpWidgetTree();
    NCWidget * firstChild = dynamic_cast<NCWidget *>(YDialog::currentDialog()->firstChild());
    if ( firstChild )
	NCMIL << "FIRST child: " << firstChild << endl;
    NCMIL << "Selector: " << w << endl;
    // FIXME ???
    return (YPackageSelector *)(w);
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPackageSelectorPluign::createPkgSpecial
//	METHOD TYPE : YWidget
//
//	DESCRIPTION : creates special widgets used for the package selection
//		      dialog (which do not have a corresponding widget in qt-ui)
//
YWidget * NCPackageSelectorPluginImpl::createPkgSpecial( YWidget *parent, const string &subwidget )
{
    YWidget * w = 0;
    YTableHeader * tableHeader = new YTableHeader();
    
    if ( subwidget == "pkgTable" )
    {
	NCDBG << "Creating a NCPkgTable" << endl;
	try
	{
	    //UIERR << "Tady taky nic neni " << endl;	
	    w = new NCPkgTable( parent, tableHeader );
	}
	catch (const std::exception & e)
	{
	    UIERR << "Caught a std::exception: " << e.what () << endl;
	}
	catch (...)
	{
	    UIERR << "Caught an unspecified exception" << endl;
	}
    }
    else
    {
	NCERR <<  "PkgSpecial( "  << subwidget << " )  not found - take default `Label" << endl;
	w = new NCLabel( parent, subwidget, false, false );
    }

    return w;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPackageSelectorPLugin::runPkgSelection
//	METHOD TYPE : YEvent *
//
//	DESCRIPTION : Implementation of UI builtin RunPkgSelection() which
//		      has to be called after OpenDialog( `PackageSelector() ).
//
YEvent * NCPackageSelectorPluginImpl::runPkgSelection(  YDialog * dialog,
						    YWidget * selector )
{
    NCPackageSelectorStart * ncSelector = 0;

    NCMIL << "Calling runPkgSelection()" << endl;
    
    if ( !dialog )
    {
	UIERR << "ERROR package selection: No dialog existing." << endl;
	return 0;
    }
    if ( !selector )
    {
	UIERR << "ERROR package selection: No package selector existing." << endl;
	return 0;
    }
    
    ncSelector = dynamic_cast<NCPackageSelectorStart *>( selector );

    bool result = true;

    // start event loop
    NCursesEvent event = NCursesEvent::cancel;
    NCDialog * ncd = static_cast<NCDialog *>( dialog );

    if ( ncSelector )
    {
	try
	{
	    ncSelector->showDefaultList();
	    NCMIL << "NCDialog: " << ncd << endl;
	    do
	    {
		event = ncd->userInput();
		result = ncSelector->handleEvent( event );
		NCDBG << "Result: " << (result?"true":"false") << endl;
	    }
	    while ( event != NCursesEvent::cancel && result == true );
	}
	catch (const std::exception & e)
	{
	    UIERR << "Caught a std::exception: " << e.what () << endl;
	}
	catch (...)
	{
	    UIERR << "Caught an unspecified exception" << endl;
	}
    }
    else
    {
	UIERR << "No NCPackageSelectorStart existing" << endl;
    }

    if ( event.result != "" )
    {
        // Before returning some value to the YCP client,
	// we must delete (==close) any leftover dialogs,
	// Wizard will not do it for us (#354712)
        while( YDialog::topmostDialog() != dialog ) {
		YDialog::deleteTopmostDialog();
	}
	NCMIL << "Return value: " << event.result << endl;
	return new YMenuEvent( event.result );
    }
    else
	return new YCancelEvent();
}

