/**
 * @file class_dimension.h
 * @brief DIMENSION_LINEAR class definition.
 */

#ifndef DIMENSION_LINEAR_H_
#define DIMENSION_LINEAR_H_

#include "../dimension/class_dimension.h"


class DIMENSION_LINEAR : public DIMENSION
{
private:

    std::vector<wxPoint*> constructionPoints;

public:

    DIMENSION_LINEAR( BOARD_ITEM* aParent );
    ~DIMENSION_LINEAR();

/* Specified dimension API */

    double  GetAngle() const override { return m_Angle; }
    void    SetAngle(double aAngle) const override;

    int     GetConstructionPointsNumber() override { return m_ConstructionPointsNumber; };
    bool    SetConstructionPoint(unsigned aPointNumber, wxPoint& aPos) override;

    std::vector<std::pair<wxPoint, wxPoint>>                GetLines() override;
    std::vector<std::pair<wxPoint, std::function<void()>>>  GetEditPoints() override;


/* BOARD_ITEM overridess */
    void Draw( EDA_DRAW_PANEL* panel, wxDC* DC,
               GR_DRAWMODE aColorMode, const wxPoint& offset = ZeroOffset )  override;
    void Move( const wxPoint& offset )                                       override;
    void Rotate( const wxPoint& aRotCentre, double aAngle )                  override;
    void Flip( const wxPoint& aCentre )                                      override;
    void Mirror( const wxPoint& axis_pos );
    void GetMsgPanelInfo( std::vector< MSG_PANEL_ITEM >& aList )             override;
    bool HitTest( const wxPoint& aPosition ) const                           override;
    bool HitTest( const EDA_RECT& aRect, bool aContained = true, int aAccuracy = 0 ) const override;

    wxString GetClass() const override { return wxT( "DIMENSION_LINEAR" ); }
    const EDA_RECT    GetBoundingBox() const                                 override;
    wxString    GetSelectMenuText() const                                    override;
    EDA_ITEM*   Clone() const                                                override;
    virtual const BOX2I ViewBBox() const                                     override;
    BITMAP_DEF GetMenuImage() const                                          override
            { return add_dimension_xpm; }

#if defined(DEBUG)
    virtual void Show( int nestLevel, std::ostream& os ) const override { ShowDummy( os ); }
#endif
};

#endif    // DIMENSION_LINEAR_H_
