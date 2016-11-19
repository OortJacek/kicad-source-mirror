/**
 * @file class_dimension.h
 * @brief DIMENSION_LINEAR class definition.
 */

#ifndef DIMENSION_LINEAR_H_
#define DIMENSION_LINEAR_H_


#include <class_board_item.h>
#include <class_pcb_text.h>
#include <class_dimension.h>


class LINE_READER;
class EDA_DRAW_PANEL;
class TEXTE_PCB;
class MSG_PANEL_ITEM;


class DIMENSION_LINEAR : public DIMENSION
{

public:

    DIMENSION_LINEAR( BOARD_ITEM* aParent );
    ~DIMENSION_LINEAR();

    void            Draw( EDA_DRAW_PANEL* panel, wxDC* DC,
                          GR_DRAWMODE aColorMode, const wxPoint& offset = ZeroOffset ) override;
    void            Move( const wxPoint& offset ) override;
    void            Rotate( const wxPoint& aRotCentre, double aAngle ) override;
    void            Flip( const wxPoint& aCentre ) override;
    void            Mirror( const wxPoint& axis_pos );
    void            GetMsgPanelInfo( std::vector< MSG_PANEL_ITEM >& aList ) override;
    bool            HitTest( const wxPoint& aPosition ) const override;
    bool HitTest( const EDA_RECT& aRect, bool aContained = true, int aAccuracy = 0 ) const override;

    wxString GetClass() const override
    {
        return wxT( "DIMENSION_LINEAR" );
    }
    const EDA_RECT    GetBoundingBox() const override;
    wxString    GetSelectMenuText() const override;
    BITMAP_DEF GetMenuImage() const override { return add_dimension_xpm; }
    EDA_ITEM*   Clone() const override;
    virtual const BOX2I ViewBBox() const override;

#if defined(DEBUG)
    virtual void Show( int nestLevel, std::ostream& os ) const override { ShowDummy( os ); }
#endif
};

#endif    // DIMENSION_LINEAR_H_
