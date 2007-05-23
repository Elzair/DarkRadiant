#ifndef SOUNDSHADERPREVIEW_H_
#define SOUNDSHADERPREVIEW_H_

#include <string>

// Forward declaration
typedef struct _GtkWidget GtkWidget;
typedef struct _GtkListStore GtkListStore;
typedef struct _GtkTreeSelection GtkTreeSelection;

namespace ui {

/** greebo: This class provides the UI elements to inspect a given
 * 			sound shader with playback option.
 * 
 * 			Use the GtkWidget* cast operator to pack this into a
 * 			parent container. 
 */
class SoundShaderPreview
{
	// The main container widget of this preview
	GtkWidget* _widget;
	
	// Tree store and view for available sound files, and the tree selection
	GtkListStore* _listStore;
	GtkWidget* _treeView;
	GtkTreeSelection* _treeSelection;
	
	// The currently "previewed" soundshader
	std::string _soundShader;
	
public:
	SoundShaderPreview();

	/** greebo: Sets the soundshader to preview. 
	 * 			This updates the preview liststore and treeview. 
	 */
	void setSoundShader(const std::string& soundShader);

	/** greebo: Operator cast to GtkWidget to pack this into a 
	 * 			parent container widget.
	 */
	operator GtkWidget*();

private:
	/** greebo: Updates the list according to the active soundshader
	 */
	void update();
};

} // namespace ui

#endif /*SOUNDSHADERPREVIEW_H_*/
