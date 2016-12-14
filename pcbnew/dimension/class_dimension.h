/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2004 Jean-Pierre Charras, jaen-pierre.charras@gipsa-lab.inpg.com
 * Copyright (C) 2016 Jacek Drewniak, jacek.drewniak@gmail.com
 * Copyright (C) 1992-2011 KiCad Developers, see AUTHORS.txt for contributors.
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
 * @file class_dimension.h
 * @brief DIMENSION class definition.
 */

#ifndef DIMENSION_H_
#define DIMENSION_H_


#include <class_board_item.h>
#include <class_pcb_text.h>

#include <vector>
#include <functional>


class LINE_READER;
class EDA_DRAW_PANEL;
class TEXTE_PCB;
class MSG_PANEL_ITEM;


enum DimShape
{
    DIM_NO_SHAPE = 0,
    DIM_LINEAR,
    DIM_ANGULAR,
    DIM_RADIAL
};

class DIMENSION : public BOARD_ITEM
{
protected:
    int         m_Width;        ///< Line width
    DimShape    m_Shape;        ///< Currently always 0.
    bool        m_DoNotChangeTextValue;
    double      m_Angle;
    EDA_UNITS_T m_Unit;         ///< 0 = inches, 1 = mm
    int         m_Value;        ///< value of PCB dimensions.
    bool 		m_outside;		///< change inside/outside arrow drawing
    bool		m_FreeText;		///< can be text moved freely
    TEXTE_PCB   m_Text;

public:

    DIMENSION( BOARD_ITEM* aParent );
    ~DIMENSION();

/* Dimension text */
    void            SetValue( int aValue );
    int             GetValue() const;

    const wxPoint&  GetTextPosition() const;
    void            SetTextPosition( const wxPoint& aPosition );

    void            SetText( const wxString& NewText );
    const wxString  GetText() const;

    TEXTE_PCB&      Text();
    TEXTE_PCB&      Text() const;

    void            SetTextSize( const wxSize& aTextSize );

/* General DIMENSION properties */
    int             GetWidth() const;
    bool            IsOutside() const;
    bool            IsFreeText() const;
    DimShape        GetDimensionShape();

    void            SetWidth( int aWidth );
    void            SetOutside( bool aOutside );
    void            SetFreeText( bool aFreeText );

/* Specified dimension API */
    virtual void Mirror( const wxPoint& axis_pos ) = 0;

    virtual double  GetAngle() const = 0;
    virtual void    SetAngle( double aAngle ) = 0;

    virtual int     GetConstructionPointsNumber() = 0;
    virtual bool    SetConstructionPoint(unsigned aPointNumber, wxPoint& aPos) = 0;

    virtual std::vector<std::pair<wxPoint, wxPoint>> GetLines() = 0;
    virtual std::vector<std::pair<wxPoint, std::function<void()>>> GetEditPoints() = 0;


/* BOARD_ITEM derivatives */
    const wxPoint&  GetPosition() const                 override;
    void            SetPosition( const wxPoint& aPos )  override;
    void            SetLayer( LAYER_ID aLayer )         override;
    wxString        GetClass() const                    override;


#if defined(DEBUG)
    virtual void Show( int nestLevel, std::ostream& os ) const override { ShowDummy( os ); }
#endif
};

#endif    // DIMENSION_H_
