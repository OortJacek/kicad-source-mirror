/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2012 Jean-Pierre Charras, jean-pierre.charras@ujf-grenoble.fr
 * Copyright (C) 2012 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 2012 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 1992-2012 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file class_dimension.cpp
 */

#include <class_dimension_linear.h>

#include <fctsys.h>
#include <macros.h>
#include <gr_basic.h>
#include <trigo.h>
#include <wxstruct.h>
#include <class_drawpanel.h>
#include <colors_selection.h>
#include <kicad_string.h>
#include <richio.h>

#include <class_board.h>
#include <class_pcb_text.h>
#include <base_units.h>


DIMENSION_LINEAR::DIMENSION_LINEAR( BOARD_ITEM* aParent ) :
    DIMENSION( aParent )
{
    m_Shape = DIM_LINEAR;
    m_DrawingPointsNumber = 3;
}


DIMENSION_LINEAR::~DIMENSION_LINEAR()
{
}


void DIMENSION_LINEAR::Move( const wxPoint& offset )
{
    m_Text.SetTextPosition( m_Text.GetTextPosition() + offset );
    m_crossBarO     += offset;
    m_crossBarOOut  += offset;
    m_crossBarF     += offset;
    m_crossBarFOut  += offset;
    m_featureLineGO += offset;
    m_featureLineGF += offset;
    m_featureLineDO += offset;
    m_featureLineDF += offset;
    m_arrowG1F  += offset;
    m_arrowG2F  += offset;
    m_arrowD1F  += offset;
    m_arrowD2F  += offset;
}


void DIMENSION_LINEAR::Rotate( const wxPoint& aRotCentre, double aAngle )
{
    wxPoint tmp = m_Text.GetTextPosition();
    RotatePoint( &tmp, aRotCentre, aAngle );
    m_Text.SetTextPosition( tmp );

    double newAngle = m_Text.GetOrientation() + aAngle;

    if( newAngle >= 3600 )
        newAngle -= 3600;

    if( newAngle > 900  &&  newAngle < 2700 )
        newAngle -= 1800;

    m_Text.SetOrientation( newAngle );

    RotatePoint( &m_crossBarO, aRotCentre, aAngle );
    RotatePoint( &m_crossBarF, aRotCentre, aAngle );
    RotatePoint( &m_featureLineGO, aRotCentre, aAngle );
    RotatePoint( &m_featureLineGF, aRotCentre, aAngle );
    RotatePoint( &m_featureLineDO, aRotCentre, aAngle );
    RotatePoint( &m_featureLineDF, aRotCentre, aAngle );
    RotatePoint( &m_arrowG1F, aRotCentre, aAngle );
    RotatePoint( &m_arrowG2F, aRotCentre, aAngle );
    RotatePoint( &m_arrowD1F, aRotCentre, aAngle );
    RotatePoint( &m_arrowD2F, aRotCentre, aAngle );


}


void DIMENSION_LINEAR::Flip( const wxPoint& aCentre )
{
    Mirror( aCentre );

    // DIMENSION_LINEAR items are not usually on copper layers, so
    // copper layers count is not taken in accoun in Flip transform
    SetLayer( FlipLayer( GetLayer() ) );
}


void DIMENSION_LINEAR::Mirror( const wxPoint& axis_pos )
{
    wxPoint newPos = m_Text.GetTextPosition();

#define INVERT( pos ) (pos) = axis_pos.y - ( (pos) - axis_pos.y )
    INVERT( newPos.y );

    m_Text.SetTextPosition( newPos );

    // invert angle
    m_Text.SetOrientation( -m_Text.GetOrientation() );

    INVERT( m_crossBarO.y );
    INVERT( m_crossBarF.y );
    INVERT( m_featureLineGO.y );
    INVERT( m_featureLineGF.y );
    INVERT( m_featureLineDO.y );
    INVERT( m_featureLineDF.y );
    INVERT( m_arrowG1F.y );
    INVERT( m_arrowG2F.y );
    INVERT( m_arrowD1F.y );
    INVERT( m_arrowD2F.y );
}

void DIMENSION_LINEAR::SetTextPosition( const wxPoint& aPosition)
{
	if(IsFreeText())
	{
		m_Text.SetTextPosition(aPosition);
	}
}

void DIMENSION_LINEAR::SetOrigin( const wxPoint& aOrigin )
{
    m_featureLineGO = aOrigin;
    AdjustDimensionDetails();
}


void DIMENSION_LINEAR::SetEnd( const wxPoint& aEnd )
{
    m_featureLineDO = aEnd;
    AdjustDimensionDetails();

}


void DIMENSION_LINEAR::SetHeight( int aHeight )
{
    m_Height = aHeight;
    AdjustDimensionDetails();
}

void DIMENSION_LINEAR::SetOutside( bool aOutside)
{
	m_outside = aOutside;
    AdjustDimensionDetails();
}

void DIMENSION_LINEAR::SetFreeText( bool aFreeText )
{
	m_FreeText = aFreeText;
	if(!m_FreeText)
	    AdjustDimensionDetails();
}



void DIMENSION_LINEAR::UpdateHeight()
{
    VECTOR2D featureLine( m_crossBarO - m_featureLineGO );
    VECTOR2D crossBar( m_featureLineDO - m_featureLineGO );

    if( featureLine.Cross( crossBar ) > 0 )
        m_Height = -featureLine.EuclideanNorm();
    else
        m_Height = featureLine.EuclideanNorm();
}


void DIMENSION_LINEAR::AdjustDimensionDetails( bool aDoNotChangeText )
{
    const int   arrowz = Mils2iu( 50 );             // size of arrows
    const int	arrowt = Mils2iu( 70 );			// size of arrow tail
    int         ii;
    int         measure, deltax, deltay;            // value of the measure on X and Y axes
    int         arrow_up_X  = 0, arrow_up_Y = 0;    // coordinates of arrow line /
    int         arrow_dw_X  = 0, arrow_dw_Y = 0;    // coordinates of arrow line '\'
    int         hx, hy;                             // dimension line interval
    double      angle, angle_f, angle_arrow;
    wxString    msg;

    // Init layer :
    m_Text.SetLayer( GetLayer() );

    // calculate the size of the dimension (text + line above the text)
    ii = m_Text.GetSize().y + m_Text.GetThickness() + (m_Width * 3);

    deltax  = m_featureLineDO.x - m_featureLineGO.x;
    deltay  = m_featureLineDO.y - m_featureLineGO.y;

    // Calculate dimension value
    measure = KiROUND( hypot( deltax, deltay ) );

    angle = atan2( (double)deltay, (double)deltax );

    // Calculation of parameters X and Y dimensions of the arrows and lines.
    hx = hy = ii;

    // Taking into account the slope of the side lines.
    if( measure )
    {
        hx  = abs( KiROUND( ( (double) deltay * hx ) / measure ) );
        hy  = abs( KiROUND( ( (double) deltax * hy ) / measure ) );

        if( m_featureLineGO.x > m_crossBarO.x )
            hx = -hx;

        if( m_featureLineGO.x == m_crossBarO.x )
            hx = 0;

        if( m_featureLineGO.y > m_crossBarO.y )
            hy = -hy;

        if( m_featureLineGO.y == m_crossBarO.y )
            hy = 0;
    }

    if( IsOutside() )
        angle_arrow = 152.5;
    else
        angle_arrow = 27.5;

    angle_f     = angle + DEG2RAD( angle_arrow ) ;
    arrow_up_X  = wxRound( arrowz * cos( angle_f ) );
    arrow_up_Y  = wxRound( arrowz * sin( angle_f ) );
    angle_f     = angle - DEG2RAD( angle_arrow );
    arrow_dw_X  = wxRound( arrowz * cos( angle_f ) );
    arrow_dw_Y  = wxRound( arrowz * sin( angle_f ) );



    int dx = KiROUND( m_Height * cos( angle + M_PI / 2 ) );
    int dy = KiROUND( m_Height * sin( angle + M_PI / 2 ) );
    m_crossBarO.x   = m_featureLineGO.x + dx;
    m_crossBarO.y   = m_featureLineGO.y + dy;
    m_crossBarF.x   = m_featureLineDO.x + dx;
    m_crossBarF.y   = m_featureLineDO.y + dy;


    dx = KiROUND( arrowt * cos( angle) );
    dy = KiROUND( arrowt * sin( angle) );
    m_crossBarFOut.x = m_crossBarF.x + dx;
    m_crossBarFOut.y = m_crossBarF.y + dy;
    m_crossBarOOut.x = m_crossBarO.x - dx;
    m_crossBarOOut.y = m_crossBarO.y - dy;

    m_arrowG1F.x    = m_crossBarO.x + arrow_up_X;
    m_arrowG1F.y    = m_crossBarO.y + arrow_up_Y;

    m_arrowG2F.x    = m_crossBarO.x + arrow_dw_X;
    m_arrowG2F.y    = m_crossBarO.y + arrow_dw_Y;

    /* The right arrow is symmetrical to the left.
     *  / = -\  and  \ = -/
     */
    m_arrowD1F.x    = m_crossBarF.x - arrow_dw_X;
    m_arrowD1F.y    = m_crossBarF.y - arrow_dw_Y;

    m_arrowD2F.x    = m_crossBarF.x - arrow_up_X;
    m_arrowD2F.y    = m_crossBarF.y - arrow_up_Y;

    m_featureLineGF.x   = m_crossBarO.x + hx;
    m_featureLineGF.y   = m_crossBarO.y + hy;

    m_featureLineDF.x   = m_crossBarF.x + hx;
    m_featureLineDF.y   = m_crossBarF.y + hy;

    // Calculate the better text position and orientation:
    if(!IsFreeText())
    {
       wxPoint textPos;
       textPos.x  = (m_crossBarF.x + m_featureLineGF.x) / 2;
       textPos.y  = (m_crossBarF.y + m_featureLineGF.y) / 2;
       m_Text.SetTextPosition( textPos );
    }

    double newAngle = -RAD2DECIDEG( angle );

    NORMALIZE_ANGLE_POS( newAngle );

    if( newAngle > 900  &&  newAngle < 2700 )
        newAngle -= 1800;

    m_Text.SetOrientation( newAngle );

    if( !aDoNotChangeText )
    {
        m_Value = measure;
        msg     = ::CoordinateToString( m_Value );
        SetText( msg );
    }
}


void DIMENSION_LINEAR::Draw( EDA_DRAW_PANEL* panel, wxDC* DC, GR_DRAWMODE mode_color,
                      const wxPoint& offset )
{
    EDA_COLOR_T gcolor;
    BOARD*      brd = GetBoard();

    if( brd->IsLayerVisible( m_Layer ) == false )
        return;

    m_Text.Draw( panel, DC, mode_color, offset );

    gcolor = brd->GetLayerColor( m_Layer );

    GRSetDrawMode( DC, mode_color );
    DISPLAY_OPTIONS* displ_opts = (DISPLAY_OPTIONS*)panel->GetDisplayOptions();
    bool filled = displ_opts ? displ_opts->m_DisplayDrawItemsFill : FILLED;
    int width   = m_Width;

    if( filled )
    {
        GRLine( panel->GetClipBox(), DC, m_crossBarO + offset,
                m_crossBarF + offset, width, gcolor );
        GRLine( panel->GetClipBox(), DC, m_featureLineGO + offset,
                m_featureLineGF + offset, width, gcolor );
        GRLine( panel->GetClipBox(), DC, m_featureLineDO + offset,
                m_featureLineDF + offset, width, gcolor );
        GRLine( panel->GetClipBox(), DC, m_crossBarF + offset,
                m_arrowD1F + offset, width, gcolor );
        GRLine( panel->GetClipBox(), DC, m_crossBarF + offset,
                m_arrowD2F + offset, width, gcolor );
        GRLine( panel->GetClipBox(), DC, m_crossBarO + offset,
                m_arrowG1F + offset, width, gcolor );
        GRLine( panel->GetClipBox(), DC, m_crossBarO + offset,
                m_arrowG2F + offset, width, gcolor );
    }
    else
    {
        GRCSegm( panel->GetClipBox(), DC, m_crossBarO + offset,
                 m_crossBarF + offset, width, gcolor );
        GRCSegm( panel->GetClipBox(), DC, m_featureLineGO + offset,
                 m_featureLineGF + offset, width, gcolor );
        GRCSegm( panel->GetClipBox(), DC, m_featureLineDO + offset,
                 m_featureLineDF + offset, width, gcolor );
        GRCSegm( panel->GetClipBox(), DC, m_crossBarF + offset,
                 m_arrowD1F + offset, width, gcolor );
        GRCSegm( panel->GetClipBox(), DC, m_crossBarF + offset,
                 m_arrowD2F + offset, width, gcolor );
        GRCSegm( panel->GetClipBox(), DC, m_crossBarO + offset,
                 m_arrowG1F + offset, width, gcolor );
        GRCSegm( panel->GetClipBox(), DC, m_crossBarO + offset,
                 m_arrowG2F + offset, width, gcolor );
    }
}


// see class_cotation.h
void DIMENSION_LINEAR::GetMsgPanelInfo( std::vector< MSG_PANEL_ITEM >& aList )
{
    // for now, display only the text within the DIMENSION_LINEAR using class TEXTE_PCB.
    m_Text.GetMsgPanelInfo( aList );
}


bool DIMENSION_LINEAR::HitTest( const wxPoint& aPosition ) const
{
    if( m_Text.TextHitTest( aPosition ) )
        return true;

    int dist_max = m_Width / 2;

    // Locate SEGMENTS

    if( TestSegmentHit( aPosition, m_crossBarO, m_crossBarF, dist_max ) )
        return true;

    if( TestSegmentHit( aPosition, m_featureLineGO, m_featureLineGF, dist_max ) )
        return true;

    if( TestSegmentHit( aPosition, m_featureLineDO, m_featureLineDF, dist_max ) )
        return true;

    if( TestSegmentHit( aPosition, m_crossBarF, m_arrowD1F, dist_max ) )
        return true;

    if( TestSegmentHit( aPosition, m_crossBarF, m_arrowD2F, dist_max ) )
        return true;

    if( TestSegmentHit( aPosition, m_crossBarO, m_arrowG1F, dist_max ) )
        return true;

    if( TestSegmentHit( aPosition, m_crossBarO, m_arrowG2F, dist_max ) )
        return true;

    return false;
}


bool DIMENSION_LINEAR::HitTest( const EDA_RECT& aRect, bool aContained, int aAccuracy ) const
{
    EDA_RECT arect = aRect;
    arect.Inflate( aAccuracy );

    EDA_RECT rect = GetBoundingBox();
    if( aAccuracy )
        rect.Inflate( aAccuracy );

    if( aContained )
        return arect.Contains( rect );

    return arect.Intersects( rect );
}


const EDA_RECT DIMENSION_LINEAR::GetBoundingBox() const
{
    EDA_RECT    bBox;
    int         xmin, xmax, ymin, ymax;

    bBox    = m_Text.GetTextBox( -1 );
    xmin    = bBox.GetX();
    xmax    = bBox.GetRight();
    ymin    = bBox.GetY();
    ymax    = bBox.GetBottom();

    xmin    = std::min( xmin, m_crossBarO.x );
    xmin    = std::min( xmin, m_crossBarF.x );
    ymin    = std::min( ymin, m_crossBarO.y );
    ymin    = std::min( ymin, m_crossBarF.y );
    xmax    = std::max( xmax, m_crossBarO.x );
    xmax    = std::max( xmax, m_crossBarF.x );
    ymax    = std::max( ymax, m_crossBarO.y );
    ymax    = std::max( ymax, m_crossBarF.y );

    xmin    = std::min( xmin, m_featureLineGO.x );
    xmin    = std::min( xmin, m_featureLineGF.x );
    ymin    = std::min( ymin, m_featureLineGO.y );
    ymin    = std::min( ymin, m_featureLineGF.y );
    xmax    = std::max( xmax, m_featureLineGO.x );
    xmax    = std::max( xmax, m_featureLineGF.x );
    ymax    = std::max( ymax, m_featureLineGO.y );
    ymax    = std::max( ymax, m_featureLineGF.y );

    xmin    = std::min( xmin, m_featureLineDO.x );
    xmin    = std::min( xmin, m_featureLineDF.x );
    ymin    = std::min( ymin, m_featureLineDO.y );
    ymin    = std::min( ymin, m_featureLineDF.y );
    xmax    = std::max( xmax, m_featureLineDO.x );
    xmax    = std::max( xmax, m_featureLineDF.x );
    ymax    = std::max( ymax, m_featureLineDO.y );
    ymax    = std::max( ymax, m_featureLineDF.y );

    bBox.SetX( xmin );
    bBox.SetY( ymin );
    bBox.SetWidth( xmax - xmin + 1 );
    bBox.SetHeight( ymax - ymin + 1 );

    bBox.Normalize();

    return bBox;
}


wxString DIMENSION_LINEAR::GetSelectMenuText() const
{
    wxString text;
    text.Printf( _( "Dimension \"%s\" on %s" ),
                GetChars( GetText() ), GetChars( GetLayerName() ) );

    return text;
}


const BOX2I DIMENSION_LINEAR::ViewBBox() const
{
    BOX2I dimBBox = BOX2I( VECTOR2I( GetBoundingBox().GetPosition() ),
                           VECTOR2I( GetBoundingBox().GetSize() ) );
    dimBBox.Merge( m_Text.ViewBBox() );

    return dimBBox;
}


EDA_ITEM* DIMENSION_LINEAR::Clone() const
{
    return new DIMENSION_LINEAR( *this );
}