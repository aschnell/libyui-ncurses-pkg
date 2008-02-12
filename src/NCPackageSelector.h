/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                        (C) SuSE GmbH |
\----------------------------------------------------------------------/

   File:       NCPackageSelector.h

   Author:     Gabriele Strattner <gs@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef NCPackageSelector_h
#define NCPackageSelector_h

#include <iosfwd>
#include "Y2Log.h"

#include "YNCursesUI.h"
#include "NCPkgNames.h"

#include <map>          
#include <string>
#include <utility>      // for STL pair

#include <YRpmGroupsTree.h>
#include "YWidgetID.h"

#include <zypp/ResObject.h>
#include <zypp/ui/Selectable.h>
#include <zypp/Patch.h>

#include "NCPkgPopupTree.h"
#include "NCPkgTable.h"
#include "NCPkgSelMapper.h"

class NCPkgPopupSelection;
class NCPkgPopupRepo;
class LangCode;
class NCPkgPopupDeps;
class NCPkgPopupDiskspace;
class NCPkgPopupSearch;
class NCPkgPopupFile;
class NCPushButton;
class YNCursesUI;
class NCMenuButton;
class YReplacePoint;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : NCPackageSelector
//
//	DESCRIPTION : holds the data and handles events
//
class NCPackageSelector
{

  friend std::ostream & operator<<( std::ostream & STREAM, const NCPackageSelector & OBJ );

  NCPackageSelector & operator=( const NCPackageSelector & );
  NCPackageSelector            ( const NCPackageSelector & );

  private:

    // typedef for the pointer to handler member function
    typedef bool (NCPackageSelector::* tHandlerFctPtr) ( const NCursesEvent& event );

    // typedef for the internal map: key=nameId, value=handler-fct-ptr
    typedef std::map<std::string, tHandlerFctPtr> tHandlerMap;

    tHandlerMap eventHandlerMap;    	// event handler map
    
    YNCursesUI * y2ui;			// the UI

    YWidget * widgetRoot;		// the root of the widget tree of the layout
    
    NCPkgPopupTree * filterPopup;	// the rpm group tags popup

    NCPkgPopupDeps * depsPopup;		// the package dependeny popup

    NCPkgPopupSelection * selectionPopup;	// the selections popup
    NCPkgPopupSelection * patternPopup;    	// the pattern popup
    NCPkgPopupSelection * languagePopup;	// language popup
    NCPkgPopupRepo * repoPopup;

    NCPkgPopupDiskspace * diskspacePopup;	// the popup showing the disk usage

    NCPkgPopupSearch * searchPopup; 	// the package search popup

    bool youMode;			// YOU
    bool updateMode;			// Update
    bool testMode;			// testing
    
    bool autoCheck;			// flag for automatic dependency check on/off
    YRpmGroupsTree * _rpmGroupsTree;	// rpm groups of the found packages

    // the package table
    NCPkgTable * pkgList;

    // filter menu and items
    NCMenuButton * filterMenu;
    YMenuItem * groupsItem;
    YMenuItem * patternsItem;
    YMenuItem * languagesItem;
    YMenuItem * reposItem;
    YMenuItem * searchItem;
    YMenuItem * installedItem;
    YMenuItem * whatifItem;
    YMenuItem * updatelistItem;
    
    // action menu and items
    NCMenuButton * actionMenu;
    YMenuItem * toggleItem;
    YMenuItem * selectItem;
    YMenuItem * deleteItem;
    YMenuItem * updateItem;
    YMenuItem * tabooItem;
    YMenuItem * notabooItem;
    YMenuItem * allItem;
    YMenuItem * selallItem;
    YMenuItem * delallItem;
    YMenuItem * deselallItem;
    YMenuItem * dontdelItem;
    YMenuItem * updnewerItem;
    YMenuItem * updallItem;
    YMenuItem * dontupdItem;
    
    // information menu and items
    NCMenuButton * infoMenu;
    YMenuItem * pkginfoItem;
    YMenuItem * longdescrItem;
    YMenuItem * versionsItem;
    YMenuItem * filesItem;
    YMenuItem * relationsItem;

    // information menu and items YOU
    YMenuItem * patchdescrItem;
    YMenuItem * patchpkgsItem;
    YMenuItem * pkgversionsItem;

    // etc menu and items
    NCMenuButton * etcMenu;
    YMenuItem * depsItem;
    YMenuItem * showdepsItem;
    YMenuItem * autodepsItem;
    YMenuItem * noautodepsItem;
    YMenuItem * verifyItem;
    YMenuItem * allpksItem;
    YMenuItem * exportItem;
    YMenuItem * importItem ;
    YMenuItem * testcaseItem;
    
    
    // labels
    YLabel * filterLabel;
    YLabel * diskspaceLabel;

    // information about packages
    NCRichText * infoText;	// short/longdecsription, filelist
    YReplacePoint * replacePoint; // replace point for info text
    
    NCPkgTable * versionsList;	// list of available package versions
    // information about patches
    NCPkgTable * patchPkgs;	// pakages belonging to a patch
    NCPkgTable * patchPkgsVersions;	// versions of packages above 
    
    NCPushButton * okButton;
    NCPushButton * cancelButton;
    
    YMenuItem * visibleInfo;		// current visible package info (description, file list, ...)
    
    // internal helper functions (format list of string) 
    string createRelLine( const zypp::CapSet & info );
    // internal use (copies tree items got from YPkgRpmGroupTagsFilterView)
    void cloneTree( YStringTreeItem * parentOrig, YTreeItem * parentClone );

    // returns the package table widget 
    NCPkgTable * getPackageList();

    // Mapping from ZyppPkg to the correspoinding ZyppSel.
    NCPkgSelMapper selMapper;

    set<string> verified_pkgs;

  protected:

    string getButtonId( YWidget *button );
    string getMenuId( YMenuItem *menu );

  public:

   /**
     * The package selector class handles the events and holds the
     * data needed for the package selection.
     * @param ui The NCurses UI
     * @param opt The widget options
     */
    NCPackageSelector( YNCursesUI * ui, YWidget * wRoot, long modeFlags );

    /**
     * Destructor
     */ 
    virtual ~NCPackageSelector();

    /**
    * Create all NCPkgPopus
    * @return void
    */
    void createPopups();

   /**
    * Create layout for the PackageSelector
    * @param parent Parent is PackageSelectorStart
    * @param type   The package table type	
    * @return void
    */
    void createPkgLayout( YWidget * parent, NCPkgTable::NCPkgTableType type );
    void createEtcMenu();

    /**
    * Create layout for the Online Update
    * @param parent Parent is PackageSelectorStart
    * @param type   The package table type	
    * @return void
    */
    void createYouLayout( YWidget * parent, NCPkgTable::NCPkgTableType type ); 	


   /**
    * Fills the package table
    * @param label The selected RPM group (the label)
    * @param group The rpm group
    * @return bool
    */
    bool fillPackageList( const string & label, YStringTreeItem * group );


  /**
    * Fills the package table with YOU patches matching the filter
    * @param filter
    * @return bool
    */
    bool fillPatchList( string filter );
			
   /**
    * Fills the package table with packages with update problems
    * @return bool
    */
    bool fillUpdateList( );

  /**
    * Fills the package table with a summary packages 
    * @return bool
    */
    bool fillSummaryList( NCPkgTable::NCPkgTableListType type );
    
   /**
    * Fills the list of available packages
    * @param table  The table widget
    * @param selectable Show all available versions of this package 
    * @return bool
    */
    bool fillAvailableList( NCPkgTable *table, ZyppSel selectable );

   /**
    * Fills the list of packages belonging to the youPatch
    * @param pkgTable  The table widget
    * @param youPatch Show all packages belonging to the patch
    * @param versions Show all versions of all packages belonging to the patch
    * @return bool
    */ 
    bool fillPatchPackages ( NCPkgTable * pkgTable, ZyppObj youPatch, bool versions = false );
    
   /**
    * Fills the package table with packages matching the search expression
    * @param expr The search expression
    * @param ignoreCase Ignore case (true or false)
    * @param checkName Search in package name (true or false)
    * @param checkSummary Check the summary (true or false)
    * @param checkProvides Check in Provides (true or false)
    * @param checkRequires Check in Requires (true or false)
    * @return bool
    */ 
    bool fillSearchList( const string & expr,
			 bool ignoreCase,
			 bool checkName,
			 bool checkSummary,
			 bool checkDescr,
			 bool checkProvides,
			 bool checkRequires );

    bool fillPatchSearchList( const string & expr );

    bool fillDefaultList();
    
    /**
      *Fills the package table with packages from selected repository
      *@param repo zypp::Repository
      */
    bool fillRepoFilterList ( ZyppRepo repo );
    
    bool isYouMode() { return youMode; }
   /**
    * Gets default RPM group (the first group)
    * @return YStringTreeItem *
    */
    YStringTreeItem * getDefaultRpmGroup();

    /**
     * @return the rpm groups
     */
    const YRpmGroupsTree * rpmGroupsTree () const { return _rpmGroupsTree; }

    /**
     * Handle the given event. For the given event (the widget-id
     * is contained in the event) the corresponding handler is executed.
     * @param event The NCurses event
     * @return bool
     */
    bool handleEvent( const NCursesEvent& event );

   /**
    * Handler function for "Search button pressed"
    * @param event The Ncurses event
    * @return bool
    */
    bool SearchHandler ( const NCursesEvent& event );

    /**
    * Handler function for menu selection "Etc./Check dependencies"
    * @param event The Ncurses event
    * @return bool
    */
    bool DependencyHandler( const NCursesEvent&  event );

     /**
     * Handler function for the "Information" menu
     * @param event The Ncurses event
     * @return bool
     */
    bool InformationHandler ( const NCursesEvent& event );

     /**
     * Generate test case /var/log/YaST2/solverTestcase
     * @return bool
     */
    bool TestcaseHandler ( const NCursesEvent& event );

    /**
     * Export/Import list of all packages and patterns to/from file
     *
     */
    bool FileHandler ( const NCursesEvent& event );

    /**
    * Handler function for "OK button pressed"
    * @param event The Ncurses event
    * @return bool 
    */
    bool OkButtonHandler ( const NCursesEvent& event );

   /**
    * Handler function for "Cancel button pressed"
    * @param event The Ncurses event
    * @return bool
    */
    bool CancelHandler ( const NCursesEvent& event );

    /**
    * Handler function for "Diskspace button pressed"
    * @param event The Ncurses event
    * @return bool
    */
    bool DiskinfoHandler ( const NCursesEvent& event );
    
    /**
     * Handler function for the "Filter" menu
     * @param event The Ncurses event
     * @return bool     
     */
    bool FilterHandler( const NCursesEvent&  event );

    /**
     * Handler function for the "Action" menu (changes the package status)
     * @param event The Ncurses event
     * @return bool     
     */
    bool StatusHandler( const NCursesEvent&  event );

   /**
    * Handler function for the "Help" menu
    * @param event The Ncurses event
    * @return bool
    */
    bool HelpHandler( const NCursesEvent&  event );

  /**
    * Handler function for the "Help" button in YOU
    * @param event The Ncurses event
    * @return bool
    */
    bool YouHelpHandler( const NCursesEvent&  event );

   /**
     * Handles hyperlinks in package description
     * @param link The link
     * @return bool
     */
    bool LinkHandler ( string link );
    
   /**
    * Gets the required package info from package manager and shows it
    * (	called from NCPkgTable )
    * @param pkgPtr the data pointer
    * @return bool
    */
    bool showPackageInformation ( ZyppObj pkgPtr, ZyppSel slbPtr );

    /**
    * Checks and shows the dependencies
    * @param doit true: do the check, false: only check if auto check is on
    */
    bool showPackageDependencies ( bool doit );

    /**
    * Verifies and shows the dependencies
    */
    bool verifyPackageDependencies ();
    
    /**
    * Checks and shows the selectiondependencies
    */
    void showSelectionDependencies ( );
    

   /**
    * Gets the required patch info from you patch manager and shows it
    * @param pkgPtr the data pointer
    * @return bool
    */
    bool showPatchInformation ( ZyppObj pkgPtr, ZyppSel slbPtr );

    
   /**
    * Sets the member variable to the currently visible information
    * @param info
    */
    void setVisibleInfo( YMenuItem * info );

   /**
    * Fills the package table
    * @param label The selected selection (the label)
    * @param wanted which packages
    * @return bool
    */
    bool showSelPackages( const string & label, const set<string> & wanted );

   /**
    * Updates the status in list of packages
    */
    void updatePackageList();
    
    /**
     * Check if 'pkg' matches 'selectedRpmGroup'.
     * Returns true if there is a match, false otherwise or if 'pkg' is 0.
     * @return bool 
     **/
    bool checkPackage( ZyppObj pkg, ZyppSel slb, YStringTreeItem * rpmGroup );

    /**
     * Check if 'patch' matches the selected filter.
     * Returns true if there is a match, false otherwise or if 'patch' is 0.
     * @return bool
     **/
    bool checkPatch( ZyppPatch patch, ZyppSel selectable, string filter );
   
   /**
    * Set status of a selectable according to its presence   
    * in the list imported from xml file (zypp::syscontent::Reader)
    * 
    * @param selectable a selectable
    * @param isWanted   should be installed or not
    * @param kind "package" or "pattern" 	
    * @return void	
    **/
    void importSelectable ( ZyppSel selectable, bool isWanted, const char* kind );
 
   /**
    * Returns whether automatic dependency is on or off
    * @return bool   
    */ 
    bool autoChecking() { return autoCheck; }

    /**
     * Creates a text from a list of strings
     * @param oneline	true: create one line, items seperated by comma; false: every string is a line
     * @return string	The text
     */
    string createText( list<string> info, bool oneline );

   /**
     * Creates a text from a list of strings which may contain HTML tags
     * @param t used to be list, now a single string. but what does it contain?
     * @return string	The text
     */
    string createDescrText( zypp::Text t );

    /**
     * Used for package search
     * @param s1 Search in s1
     * @param s2 Searching for s2
     * @return bool
     */
    bool match( string s1, string s2, bool ignoreCase );

    /**
     * Calls the package mananager (updateDu()) and shows the required disk space
     */ 
    void showDiskSpace();

    /**
     * Shows the total download size
     */ 
    void showDownloadSize();

    /**
     * Check for changes
     */
    void saveState();
    void restoreState();
    bool diffState();

   /**
     * Check for license
     */
    bool showPendingLicenseAgreements();
    bool showPendingLicenseAgreements( ZyppPoolIterator begin, ZyppPoolIterator end );

   /**
    * Show popup with license.
    * @return bool
    */   
    bool showLicenseAgreement( ZyppSel & slbPtr , string licenseText );
    
    void createFilterMenu();

   /**
    * Get list of packages already selected for automatic changes
    * (usually via 'verify system' call)
    * @return std::set <string>
    */
    set <string> getVerifiedPkgs() 
    {
	return verified_pkgs;
    }

    /**
     * Insert package name into the list of already selected for automatic changes
     * @param pkgname Package name
     */
    void insertVerifiedPkg( string pkgname ) {
	verified_pkgs.insert( pkgname);
    }

    /**
     * Empty the set of packages selected for automatic changes
     * @return void
     */
    void clearVerifiedPkgs() {
	if ( !verified_pkgs.empty() )
	{
	    NCMIL << "Discarding auto-dependency changes" << endl;
	    verified_pkgs.clear();
	}
    }

};

///////////////////////////////////////////////////////////////////

#endif // NCPackageSelector_h
