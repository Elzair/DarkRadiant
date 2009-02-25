#ifndef SREDITOR_H_
#define SREDITOR_H_

#include "ientity.h"
#include "iradiant.h"
#include "icommandsystem.h"
#include "iscenegraph.h"
#include "gtkutil/WindowPosition.h"
#include "gtkutil/window/BlockingTransientWindow.h"

#include "StimTypes.h"
#include "SREntity.h"
#include "StimEditor.h"
#include "ResponseEditor.h"
#include "CustomStimEditor.h"

// Forward declarations
typedef struct _GtkNotebook GtkNotebook;

namespace ui {

class StimResponseEditor;
typedef boost::shared_ptr<StimResponseEditor> StimResponseEditorPtr;

class StimResponseEditor :
	public gtkutil::BlockingTransientWindow
{
	// The overall dialog vbox (used to quickly disable the whole dialog)
	GtkWidget* _dialogVBox;
	
	GtkNotebook* _notebook;
	int _stimPageNum;
	int _responsePageNum;
	int _customStimPageNum;
	static int _lastShownPage;
	
	// The close button to toggle the view
	GtkWidget* _closeButton;
	
	// The "extended" entity object managing the stims
	SREntityPtr _srEntity;
	
	// The position/size memoriser
	gtkutil::WindowPosition _windowPosition;
	
	// The entity we're editing
	Entity* _entity;
	
	// The helper class managing the stims
	StimTypes _stimTypes;
	
	// The helper classes for editing the stims/responses
	StimEditor _stimEditor;
	ResponseEditor _responseEditor;
	CustomStimEditor _customStimEditor;

public:
	StimResponseEditor();
	
	// Command target to toggle the dialog
	static void showDialog(const cmd::ArgumentList& args);

private:
	virtual void _preHide();
	virtual void _preShow();
	
	/** greebo: Saves the current working set to the entity
	 */
	void save();

	/* WIDGET POPULATION */
	void populateWindow(); 			// Main window
	GtkWidget* createButtons(); 	// Dialog buttons
	
	/** greebo: Checks the selection for a single entity.
	 */
	void rescanSelection();

	// Button callbacks
	static void onSave(GtkWidget* button, StimResponseEditor* self);
	static void onClose(GtkWidget* button, StimResponseEditor* self);

	// The keypress handler for catching the keys in the treeview
	static gboolean onWindowKeyPress(
		GtkWidget* dialog, GdkEventKey* event, StimResponseEditor* self);

}; // class StimResponseEditor

} // namespace ui

#endif /*SREDITOR_H_*/
