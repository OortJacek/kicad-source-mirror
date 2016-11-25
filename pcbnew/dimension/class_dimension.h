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
    int         m_Width;        ///< Line width
    int         m_Shape;        ///< Currently always 0.
    int         m_DrawingPointsNumber;
    double      m_Angle;
    EDA_UNITS_T m_Unit;         ///< 0 = inches, 1 = mm
    int         m_Value;        ///< value of PCB dimensions.
    bool 		m_outside;		///< change inside/outside arrow drawing
    bool		m_FreeText;		///< can be text moved freely
    TEXTE_PCB   m_Text;

public:

    DIMENSION( BOARD_ITEM* aParent );
    ~DIMENSION();

/* Dimension text value */
    void            SetValue( int aValue ) { m_Value = aValue; }
    int             GetValue() const { return m_Value; }

    const wxPoint&  GetTextPosition() const	{ return m_Text.GetTextPosition(); }
    void            SetTextPosition( const wxPoint& aPosition ) {return;}

    void            SetText( const wxString& NewText );
    const wxString  GetText() const;

    TEXTE_PCB&      Text()  { return m_Text; }
    TEXTE_PCB&      Text() const  { return *(const_cast<TEXTE_PCB*> (&m_Text)); }

    void            SetTextSize( const wxSize& aTextSize )  {m_Text.SetSize( aTextSize ); }

/* General DIMENSION properties */
    int             GetWidth() const { return m_Width; }
    void            SetWidth( int aWidth ) { m_Width = aWidth; }

    void            SetOutside( bool aOutside) { return;}
    bool            IsOutside() const { return m_outside; }

    void            SetFreeText( bool aFreeText ) {return;}
    bool            IsFreeText() const { return m_FreeText; }

    int             GetDimensionShape() {return m_Shape;}

/* Specified dimension API */
    virtual double  GetAngle() const = 0;
    virtual void    SetAngle(double aAngle) const = 0;

    virtual int     GetDrawingPointsNumber() = 0;;
    virtual bool    SetDrawingPoint(int aPointNumber, wxPoint& aPos) = 0;

    virtual std::vector<std::pair<wxPoint, wxPoint>>& GetLines() = 0;
    virtual std::vector<std::pair<wxPoint, std::function<void()>>>& GetEditPoints() = 0;


/* BOARD_ITEM derivatives */
    const wxPoint&  GetPosition() const                 override;
    void            SetPosition( const wxPoint& aPos )  override;
    void            SetLayer( LAYER_ID aLayer )         override;
    wxString        GetClass() const                    override
    {
        return wxT( "DIMENSION" );
    }

#if defined(DEBUG)
    virtual void Show( int nestLevel, std::ostream& os ) const override { ShowDummy( os ); }
#endif
};

#endif    // DIMENSION_H_
