/***************************************************************************
 *   Copyright (C) 2007 by Pino Toscano <pino@kde.org>                     *
 *   Copyright (C) 2018 by Intevation GmbH <intevation@intevation.de>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "formfields.h"

#include "core/action.h"

#include "pdfsignatureutils.h"

#include <poppler-qt5.h>

#include <config-okular-poppler.h>

extern Okular::Action* createLinkFromPopplerLink(const Poppler::Link *popplerLink, bool deletePopplerLink = true);
#ifdef HAVE_POPPLER_0_65
# define SET_ANNOT_ACTIONS \
    setAdditionalAction( Okular::Annotation::CursorEntering, createLinkFromPopplerLink( field->additionalAction( Poppler::Annotation::CursorEnteringAction ) ) ); \
    setAdditionalAction( Okular::Annotation::CursorLeaving, createLinkFromPopplerLink( field->additionalAction( Poppler::Annotation::CursorLeavingAction ) ) ); \
    setAdditionalAction( Okular::Annotation::MousePressed, createLinkFromPopplerLink( field->additionalAction( Poppler::Annotation::MousePressedAction ) ) ); \
    setAdditionalAction( Okular::Annotation::MouseReleased, createLinkFromPopplerLink( field->additionalAction( Poppler::Annotation::MouseReleasedAction ) ) ); \
    setAdditionalAction( Okular::Annotation::FocusIn, createLinkFromPopplerLink( field->additionalAction( Poppler::Annotation::FocusInAction ) ) ); \
    setAdditionalAction( Okular::Annotation::FocusOut, createLinkFromPopplerLink( field->additionalAction( Poppler::Annotation::FocusOutAction ) ) );
#else
# define SET_ANNOT_ACTIONS
#endif

#ifdef HAVE_POPPLER_0_53
#define SET_ACTIONS \
    setActivationAction( createLinkFromPopplerLink( field->activationAction() ) ); \
    setAdditionalAction( Okular::FormField::FieldModified, createLinkFromPopplerLink( field->additionalAction( Poppler::FormField::FieldModified ) ) ); \
    setAdditionalAction( Okular::FormField::FormatField, createLinkFromPopplerLink( field->additionalAction( Poppler::FormField::FormatField ) ) ); \
    setAdditionalAction( Okular::FormField::ValidateField, createLinkFromPopplerLink( field->additionalAction( Poppler::FormField::ValidateField ) ) ); \
    setAdditionalAction( Okular::FormField::CalculateField, createLinkFromPopplerLink( field->additionalAction( Poppler::FormField::CalculateField ) ) ); \
    SET_ANNOT_ACTIONS
#else
#define SET_ACTIONS \
    setActivationAction( createLinkFromPopplerLink( field->activationAction() ) );
#endif

PopplerFormFieldButton::PopplerFormFieldButton( Poppler::FormFieldButton * field )
    : Okular::FormFieldButton(), m_field( field )
{
    m_rect = Okular::NormalizedRect::fromQRectF( m_field->rect() );
    m_id = m_field->id();
    SET_ACTIONS
}

PopplerFormFieldButton::~PopplerFormFieldButton()
{
    delete m_field;
}

Okular::NormalizedRect PopplerFormFieldButton::rect() const
{
    return m_rect;
}

int PopplerFormFieldButton::id() const
{
    return m_id;
}

QString PopplerFormFieldButton::name() const
{
    return m_field->name();
}

QString PopplerFormFieldButton::uiName() const
{
    return m_field->uiName();
}

bool PopplerFormFieldButton::isReadOnly() const
{
    return m_field->isReadOnly();
}

void PopplerFormFieldButton::setReadOnly( bool value )
{
#ifdef HAVE_POPPLER_0_64
    m_field->setReadOnly( value );
#else
    Q_UNUSED( value );
#endif
}

bool PopplerFormFieldButton::isVisible() const
{
    return m_field->isVisible();
}

void PopplerFormFieldButton::setVisible( bool value )
{
#ifdef HAVE_POPPLER_0_64
    m_field->setVisible( value );
#else
    Q_UNUSED( value );
#endif
}

Okular::FormFieldButton::ButtonType PopplerFormFieldButton::buttonType() const
{
    switch ( m_field->buttonType() )
    {
        case Poppler::FormFieldButton::Push:
            return Okular::FormFieldButton::Push;
        case Poppler::FormFieldButton::CheckBox:
            return Okular::FormFieldButton::CheckBox;
        case Poppler::FormFieldButton::Radio:
            return Okular::FormFieldButton::Radio;
    }
    return Okular::FormFieldButton::Push;
}

QString PopplerFormFieldButton::caption() const
{
    return m_field->caption();
}

bool PopplerFormFieldButton::state() const
{
    return m_field->state();
}

void PopplerFormFieldButton::setState( bool state )
{
    m_field->setState( state );
}

QList< int > PopplerFormFieldButton::siblings() const
{
    return m_field->siblings();
}


PopplerFormFieldText::PopplerFormFieldText( Poppler::FormFieldText * field )
    : Okular::FormFieldText(), m_field( field )
{
    m_rect = Okular::NormalizedRect::fromQRectF( m_field->rect() );
    m_id = m_field->id();
    SET_ACTIONS
}

PopplerFormFieldText::~PopplerFormFieldText()
{
    delete m_field;
}

Okular::NormalizedRect PopplerFormFieldText::rect() const
{
    return m_rect;
}

int PopplerFormFieldText::id() const
{
    return m_id;
}

QString PopplerFormFieldText::name() const
{
    return m_field->name();
}

QString PopplerFormFieldText::uiName() const
{
    return m_field->uiName();
}

bool PopplerFormFieldText::isReadOnly() const
{
    return m_field->isReadOnly();
}

void PopplerFormFieldText::setReadOnly( bool value )
{
#ifdef HAVE_POPPLER_0_64
    m_field->setReadOnly( value );
#else
    Q_UNUSED( value );
#endif
}

bool PopplerFormFieldText::isVisible() const
{
    return m_field->isVisible();
}

void PopplerFormFieldText::setVisible( bool value )
{
#ifdef HAVE_POPPLER_0_64
    m_field->setVisible( value );
#else
    Q_UNUSED( value );
#endif
}

Okular::FormFieldText::TextType PopplerFormFieldText::textType() const
{
    switch ( m_field->textType() )
    {
        case Poppler::FormFieldText::Normal:
            return Okular::FormFieldText::Normal;
        case Poppler::FormFieldText::Multiline:
            return Okular::FormFieldText::Multiline;
        case Poppler::FormFieldText::FileSelect:
            return Okular::FormFieldText::FileSelect;
    }
    return Okular::FormFieldText::Normal;
}

QString PopplerFormFieldText::text() const
{
    return m_field->text();
}

void PopplerFormFieldText::setText( const QString& text )
{
    m_field->setText( text );
}

bool PopplerFormFieldText::isPassword() const
{
    return m_field->isPassword();
}

bool PopplerFormFieldText::isRichText() const
{
    return m_field->isRichText();
}

int PopplerFormFieldText::maximumLength() const
{
    return m_field->maximumLength();
}

Qt::Alignment PopplerFormFieldText::textAlignment() const
{
    return Qt::AlignTop | m_field->textAlignment();
}

bool PopplerFormFieldText::canBeSpellChecked() const
{
    return m_field->canBeSpellChecked();
}


PopplerFormFieldChoice::PopplerFormFieldChoice( Poppler::FormFieldChoice * field )
    : Okular::FormFieldChoice(), m_field( field )
{
    m_rect = Okular::NormalizedRect::fromQRectF( m_field->rect() );
    m_id = m_field->id();
    SET_ACTIONS
}

PopplerFormFieldChoice::~PopplerFormFieldChoice()
{
    delete m_field;
}

Okular::NormalizedRect PopplerFormFieldChoice::rect() const
{
    return m_rect;
}

int PopplerFormFieldChoice::id() const
{
    return m_id;
}

QString PopplerFormFieldChoice::name() const
{
    return m_field->name();
}

QString PopplerFormFieldChoice::uiName() const
{
    return m_field->uiName();
}

bool PopplerFormFieldChoice::isReadOnly() const
{
    return m_field->isReadOnly();
}

void PopplerFormFieldChoice::setReadOnly( bool value )
{
#ifdef HAVE_POPPLER_0_64
    m_field->setReadOnly( value );
#else
    Q_UNUSED( value );
#endif
}

bool PopplerFormFieldChoice::isVisible() const
{
    return m_field->isVisible();
}

void PopplerFormFieldChoice::setVisible( bool value )
{
#ifdef HAVE_POPPLER_0_64
    m_field->setVisible( value );
#else
    Q_UNUSED( value );
#endif
}

Okular::FormFieldChoice::ChoiceType PopplerFormFieldChoice::choiceType() const
{
    switch ( m_field->choiceType() )
    {
        case Poppler::FormFieldChoice::ComboBox:
            return Okular::FormFieldChoice::ComboBox;
        case Poppler::FormFieldChoice::ListBox:
            return Okular::FormFieldChoice::ListBox;
    }
    return Okular::FormFieldChoice::ListBox;
}

QStringList PopplerFormFieldChoice::choices() const
{
    return m_field->choices();
}

bool PopplerFormFieldChoice::isEditable() const
{
    return m_field->isEditable();
}

bool PopplerFormFieldChoice::multiSelect() const
{
    return m_field->multiSelect();
}

QList<int> PopplerFormFieldChoice::currentChoices() const
{
    return m_field->currentChoices();
}

void PopplerFormFieldChoice::setCurrentChoices( const QList<int>& choices )
{
    m_field->setCurrentChoices( choices );
}

QString PopplerFormFieldChoice::editChoice() const
{
    return m_field->editChoice();
}

void PopplerFormFieldChoice::setEditChoice( const QString& text )
{
    m_field->setEditChoice( text );
}

Qt::Alignment PopplerFormFieldChoice::textAlignment() const
{
    return Qt::AlignTop | m_field->textAlignment();
}

bool PopplerFormFieldChoice::canBeSpellChecked() const
{
    return m_field->canBeSpellChecked();
}

#ifndef HAVE_POPPLER_0_51

class DummySignatureInfo : public Okular::SignatureInfo
{
};

#endif


PopplerFormFieldSignature::PopplerFormFieldSignature( Poppler::FormFieldSignature * field )
    : Okular::FormFieldSignature(), m_field( field )
{
    m_rect = Okular::NormalizedRect::fromQRectF( m_field->rect() );
    m_id = m_field->id();
#ifdef HAVE_POPPLER_0_51
    m_info = new PopplerSignatureInfo( m_field->validate( Poppler::FormFieldSignature::ValidateVerifyCertificate ) );
#else
    m_info = new DummySignatureInfo();
#endif
    SET_ACTIONS
}

PopplerFormFieldSignature::~PopplerFormFieldSignature()
{
    delete m_field;
    delete m_info;
}

Okular::NormalizedRect PopplerFormFieldSignature::rect() const
{
    return m_rect;
}

int PopplerFormFieldSignature::id() const
{
    return m_id;
}

QString PopplerFormFieldSignature::name() const
{
    return m_field->name();
}

QString PopplerFormFieldSignature::uiName() const
{
    return m_field->uiName();
}

bool PopplerFormFieldSignature::isReadOnly() const
{
    return m_field->isReadOnly();
}

bool PopplerFormFieldSignature::isVisible() const
{
    return m_field->isVisible();
}

PopplerFormFieldSignature::SignatureType PopplerFormFieldSignature::signatureType() const
{
#ifdef HAVE_POPPLER_0_58
    switch ( m_field->signatureType() )
    {
        case Poppler::FormFieldSignature::AdbePkcs7sha1:
            return Okular::FormFieldSignature::AdbePkcs7sha1;
        case Poppler::FormFieldSignature::AdbePkcs7detached:
            return Okular::FormFieldSignature::AdbePkcs7detached;
        case Poppler::FormFieldSignature::EtsiCAdESdetached:
            return Okular::FormFieldSignature::EtsiCAdESdetached;
        default:
            return Okular::FormFieldSignature::UnknownType;
    }
#else
    return Okular::FormFieldSignature::UnknownType;
#endif
}

const Okular::SignatureInfo &PopplerFormFieldSignature::signatureInfo() const
{
    return *m_info;
}
