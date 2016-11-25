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
    m_DrawingPointsNumber = 2;
}


DIMENSION_LINEAR::~DIMENSION_LINEAR()
{
}


void DIMENSION_LINEAR::Move( const wxPoint& offset )
{
}


void DIMENSION_LINEAR::Rotate( const wxPoint& aRotCentre, double aAngle )
{

}


void DIMENSION_LINEAR::Flip( const wxPoint& aCentre )
{

}


void DIMENSION_LINEAR::Mirror( const wxPoint& axis_pos )
{

}


void DIMENSION_LINEAR::Draw( EDA_DRAW_PANEL* panel, wxDC* DC, GR_DRAWMODE mode_color,
                      const wxPoint& offset )
{

}


// see class_cotation.h
void DIMENSION_LINEAR::GetMsgPanelInfo( std::vector< MSG_PANEL_ITEM >& aList )
{
}


bool DIMENSION_LINEAR::HitTest( const wxPoint& aPosition ) const
{
    return false;
}


bool DIMENSION_LINEAR::HitTest( const EDA_RECT& aRect, bool aContained, int aAccuracy ) const
{
    return false;
}


const EDA_RECT DIMENSION_LINEAR::GetBoundingBox() const
{
    EDA_RECT    bBox;

    bBox    = m_Text.GetTextBox( -1 );
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
