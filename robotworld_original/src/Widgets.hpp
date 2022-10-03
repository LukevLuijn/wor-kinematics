#ifndef WIDGETS_HPP_
#define WIDGETS_HPP_

#include "Config.hpp"

#include "Point.hpp"
#include "Size.hpp"
#include "Region.hpp"

#include <wx/app.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/defs.h>
#include <wx/dnd.h>
#include <wx/event.h>
#include <wx/gbsizer.h>
#include <wx/gdicmn.h>
#include <wx/imaglist.h>
#include <wx/splitter.h>
#include <wx/brush.h>
#include <wx/button.h>
#include <wx/colour.h>
#include <wx/cursor.h>
#include <wx/font.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/statline.h>
#include <wx/toolbar.h>
#include <wx/listbase.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/treebase.h>
#include <wx/validate.h>
#include <wx/generic/textdlgg.h>

#include <string>

namespace Widgets
{
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_activate_event
	 */
typedef wxActivateEvent ActivateEvent;
/**
 * @see http://docs.wxwidgets.org/stable/
 */
#define ActivateEventHandler wxActivateEventHandler
/**
 * @see http://docs.wxwidgets.org/stable/
 */
	typedef wxBoxSizer BoxSizer;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_brush
	 */
	typedef wxBrush Brush;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_button
	 */
	typedef wxButton Button;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_colour
	 */
	typedef wxColour Colour;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_cursor
	 */
	typedef wxCursor Cursor;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_command_event
	 */
	typedef wxCommandEvent CommandEvent;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_close_event
	 */
	typedef wxCloseEvent CloseEvent;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_command_event_handler
	 */
#define CommandEventHandler wxCommandEventHandler
	/**
	 * @see http://http://docs.wxwidgets.org/stable/defs_8h#a11f6efc0e8d8d680f3ec8e82aa4f1770
	 */
	typedef wxCoord Coord;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_data_format
	 */
	typedef wxDataFormat DataFormat;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_default_position
	 */
#define DefaultPosition wxDefaultPosition
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_default_size
	 */
#define DefaultSize wxDefaultSize
	/**
	 * @see http://http://docs.wxwidgets.org/stable/classwx_validator
	 */
#define DefaultValidator wxDefaultValidator
	/**
	 * @see http://docs.wxwidgets.org/stable/overview_dnd
	 */
	typedef wxDragResult DragResult;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_event
	 */
	typedef wxEvent Event;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_event_handler
	 */
#define EventHandler wxEvtHandler
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_flex_grid_sizer
	 */
	typedef wxFlexGridSizer FlexGridSizer;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_focus_event
	 */
	typedef wxFocusEvent FocusEvent;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_font
	 */
	typedef wxFont Font;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_frame
	 */
	typedef wxFrame Frame;
	/**
	 * @see http://http://docs.wxwidgets.org/stable/classwx_g_b_position
	 */
	typedef wxGBPosition GBPosition;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_g_b_span
	 */
	typedef wxGBSpan GBSpan;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_grid_bag_sizer
	 */
	typedef wxGridBagSizer GridBagSizer;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_image_list
	 */
	typedef wxImageList ImageList;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_key_event
	 */
	typedef wxKeyEvent KeyEvent;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_key_event_handler
	 */
#define KeyEventHandler wxKeyEventHandler
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_list_event
	 */
	typedef wxListEvent ListEvent;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_list_event_handler
	 */
#define ListEventHandler wxListEventHandler
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_list_item
	 */
	typedef wxListItem ListItem;
	/**classwx_treeevent
	 * @see http://docs.wxwidgets.org/stable/classwx_menu_bar
	 */
	typedef wxMenuBar MenuBar;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_menu
	 */
	typedef wxMenu Menu;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_mouse_event
	 */
	typedef wxMouseEvent MouseEvent;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_mouse_event_handler
	 */
#define MouseEventHandler wxMouseEventHandler
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_notify_event
	 */
	typedef wxNotifyEvent NotifyEvent;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_colour
	 */
#define NullColour wxNullColour
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_paint_event
	 */
	typedef wxPaintEvent PaintEvent;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_paint_event_handler
	 */
#define PaintEventHandler wxPaintEventHandler
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_panel
	 */
	typedef wxPanel Panel;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_scrolled
	 */
	typedef wxScrolledCanvas ScrolledCanvas;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_scrolled
	 */
	typedef wxScrolledWindow ScrolledWindow;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_size_event
	 */
	typedef wxSizeEvent SizeEvent;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_size_event_handler
	 */
#define SizeEventHandler wxSizeEventHandler
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_sizer
	 */
	typedef wxSizer Sizer;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_sizer_flags
	 */
	typedef wxSizerFlags SizerFlags;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_splitter_window
	 */
	typedef wxSplitterWindow SplitterWindow;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_static_box_sizer
	 */
	typedef wxStaticBoxSizer StaticBoxSizer;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_static_line
	 */
	typedef wxStaticLine StaticLine;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_text_entry_dialog
	 */
	typedef wxTextEntryDialog TextEntryDialog;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_tool_bar
	 */
	typedef wxToolBar ToolBar;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_tree_event
	 */
	typedef wxTreeEvent TreeEvent;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_tree_event_handler
	 */
#define TreeEventHandler wxTreeEventHandler
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_tree_item_id
	 */
	typedef wxTreeItemId TreeItemId;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_validator
	 */
	typedef wxValidator Validator;
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_window_id
	 */
#define WindowId wxWindowID
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_window
	 */
	typedef wxWindow Window;

#define DEFAULT_ID wxID_ANY

#define FOCUS_COLOUR wxColour(240,240,200)
#define DEFAULT_FRAME_STYLE wxDEFAULT_FRAME_STYLE
#define DEFAULT_DIALOG_STYLE wxDEFAULT_DIALOG_STYLE

#define CROSS_CURSOR wxCROSS_CURSOR
#define HOURGLASS_CURSOR wxHOURGLASS_CURSOR
#define STANDARD_CURSOR wxSTANDARD_CURSOR

#define TAB_TRAVERSAL wxTAB_TRAVERSAL

#define LC_LIST wxLC_LIST
#define LC_REPORT wxLC_REPORT
#define LC_SINGLE_SEL wxLC_SINGLE_SEL

#define TR_HAS_BUTTONS wxTR_HAS_BUTTONS
#define TR_HIDE_ROOT wxTR_HIDE_ROOT

	enum GeometryCentre
	{
		CENTRE = wxCENTRE,
		CENTER = CENTRE
	};

	enum Orientation
	{
		HORIZONTAL = wxHORIZONTAL,
		DIR_HORIZONTAL = HORIZONTAL,
		VERTICAL = wxVERTICAL,
		DIR_VERTICAL = VERTICAL,

		BOTH = VERTICAL | HORIZONTAL
	};

	enum Direction
	{
		LEFT = wxLEFT,
		RIGHT = wxRIGHT,
		UP = wxUP,
		DOWN = wxDOWN,

		TOP = UP,
		BOTTOM = DOWN,

		NORTH = UP,
		SOUTH = DOWN,
		WEST = LEFT,
		EAST = RIGHT,

		ALL = (UP | DOWN | RIGHT | LEFT)
	};

	enum Alignment
	{
		ALIGN_NOT = wxALIGN_NOT,
		ALIGN_CENTER_HORIZONTAL = wxALIGN_CENTER_HORIZONTAL,
		ALIGN_CENTRE_HORIZONTAL = ALIGN_CENTER_HORIZONTAL,
		ALIGN_LEFT = ALIGN_NOT,
		ALIGN_TOP = ALIGN_NOT,
		ALIGN_RIGHT = wxALIGN_RIGHT,
		ALIGN_BOTTOM = wxALIGN_BOTTOM,
		ALIGN_CENTER_VERTICAL = wxALIGN_CENTER_VERTICAL,
		ALIGN_CENTRE_VERTICAL = ALIGN_CENTER_VERTICAL,

		ALIGN_CENTER = (ALIGN_CENTER_HORIZONTAL | ALIGN_CENTER_VERTICAL),
		ALIGN_CENTRE = ALIGN_CENTER,
		ALIGN_MASK = wxALIGN_MASK
	};

	enum Stretch
	{
		STRETCH_NOT = wxSTRETCH_NOT,
		SHRINK = wxSHRINK,
		GROW = wxGROW,
		EXPAND = GROW,
		SHAPED = wxSHAPED,
		FIXED_MINSIZE = wxFIXED_MINSIZE,
		RESERVE_SPACE_EVEN_IF_HIDDEN = wxRESERVE_SPACE_EVEN_IF_HIDDEN,
		TILE = wxTILE
	};

	inline wxString WXSTRING( const char* aText)
	{
		return wxString::FromAscii( aText);
	}

	inline wxString WXSTRING( const std::string& aString)
	{
		return wxString::FromAscii( aString.c_str());
	}

	inline wxString WXSTRING( std::string& aString)
	{
		return wxString::FromAscii( aString.c_str());
	}

	inline std::string STDSTRING( const wxString& aString)
	{
		return std::string( aString.ToAscii());
	}

	inline std::string STDSTRING( wxString& aString)
	{
		return std::string( aString.ToAscii());
	}

	/**
	 *
	 * @param aTitleBarMessage
	 * @param aMessage
	 * @param aDefaultValue
	 * @param aParent
	 * @param centre
	 * @param aPoint
	 * @return
	 */
	std::string GetTextFromUser(	const std::string& aTitleBarMessage,
									const std::string& aMessage,
									const std::string& aDefaultValue = "",
									Window* aParent = nullptr,
									bool centre = false,
									const Point& aPoint = DefaultPosition);
} // namespace Widgets
using namespace Widgets;

#endif /* WIDGETS_HPP_ */
