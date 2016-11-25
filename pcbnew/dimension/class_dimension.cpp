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

#include <dimension/class_dimension.h>

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


DIMENSION::DIMENSION( BOARD_ITEM* aParent ) :
    BOARD_ITEM( aParent, PCB_DIMENSION_T ),
    m_Width( Millimeter2iu( 0.2 ) ), m_Unit( INCHES ), m_Value( 0 ), m_Text( this )
{
    m_Layer = Dwgs_User;
    m_outside = false;
    m_FreeText = false;
    m_Angle = 0.0d;
}


DIMENSION::~DIMENSION()
{
}


void DIMENSION::SetPosition( const wxPoint& aPos )
{
    m_Text.SetTextPosition( aPos );
}


const wxPoint& DIMENSION::GetPosition() const
{
    return m_Text.GetTextPosition();
}


void DIMENSION::SetText( const wxString& aNewText )
{
    m_Text.SetText( aNewText );
}


const wxString DIMENSION::GetText() const
{
    return m_Text.GetText();
}


void DIMENSION::SetLayer( LAYER_ID aLayer )
{
    m_Layer = aLayer;
    m_Text.SetLayer( aLayer );
}

