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

    static constexpr unsigned int m_ConstructionPointsNumber = 2;
    unsigned int m_SetConstructionPoints;

    std::array<wxPoint, m_ConstructionPointsNumber> constructionPoints;

    wxPoint     m_crossBarO, m_crossBarF;
    wxPoint     m_crossBarOOut, m_crossBarFOut;
    wxPoint     m_featureLineGO, m_featureLineGF;
    wxPoint     m_featureLineDO, m_featureLineDF;
    wxPoint     m_arrowD1F, m_arrowD2F;
    wxPoint     m_arrowG1F, m_arrowG2F;
    int         m_Height;

    bool RebuildDimensionSchape();

public:

    DIMENSION_LINEAR( BOARD_ITEM* aParent );
    ~DIMENSION_LINEAR();

/* Specified dimension API */

    double  GetAngle() 												   const override;
    void    SetAngle(double aAngle)                                          override;

    int     GetConstructionPointsNumber()                                    override;
    bool    SetConstructionPoint(unsigned aPointNumber, wxPoint& aPos)       override;

    std::vector<std::pair<wxPoint, wxPoint>>                GetLines()       override;
    std::vector<std::pair<wxPoint, std::function<void()>>>  GetEditPoints()  override;


/* BOARD_ITEM overridess */

    void Move( const wxPoint& offset )                                        override;
    void Rotate( const wxPoint& aRotCentre, double aAngle )                   override;
    void Flip( const wxPoint& aCentre )                                       override;
    void Mirror( const wxPoint& axis_pos )                                    override;
    void GetMsgPanelInfo( std::vector< MSG_PANEL_ITEM >& aList )              override;
    bool HitTest( const wxPoint& aPosition )                            const override;
    bool HitTest( const EDA_RECT& aRect,
                  bool aContained = true, int aAccuracy = 0 )           const override;

    wxString GetClass()                                                 const override;
    const EDA_RECT    GetBoundingBox()                                  const override;
    wxString    GetSelectMenuText()                                     const override;
    EDA_ITEM*   Clone()                                                 const override;
    virtual const BOX2I ViewBBox()                                      const override;
    BITMAP_DEF GetMenuImage()                                           const override;

    void Draw( EDA_DRAW_PANEL* panel, wxDC* DC,
               GR_DRAWMODE aColorMode,
               const wxPoint& offset = ZeroOffset )  override;


#if defined(DEBUG)
    virtual void Show( int nestLevel, std::ostream& os ) const override { ShowDummy( os ); }
#endif
};

#endif    // DIMENSION_LINEAR_H_
