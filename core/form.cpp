/***************************************************************************
 *   Copyright (C) 2007 by Pino Toscano <pino@kde.org>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "form.h"
#include "form_p.h"

// qt includes
#include <QVariant>

#include "action.h"

using namespace Okular;

FormFieldPrivate::FormFieldPrivate( FormField::FieldType type )
    : m_type( type ), m_activateAction( nullptr )
{
}

FormFieldPrivate::~FormFieldPrivate()
{
    delete m_activateAction;
    qDeleteAll( m_additionalActions.values() );
    qDeleteAll( m_additionalAnnotActions.values() );
}

void FormFieldPrivate::setDefault()
{
    m_default = value();
}


FormField::FormField( FormFieldPrivate &dd )
    : d_ptr( &dd )
{
    d_ptr->q_ptr = this;
}

FormField::~FormField()
{
    delete d_ptr;
}

FormField::FieldType FormField::type() const
{
    Q_D( const FormField );
    return d->m_type;
}

bool FormField::isReadOnly() const
{
    return false;
}

void FormField::setReadOnly( bool )
{
}

bool FormField::isVisible() const
{
    return true;
}

void FormField::setVisible( bool )
{
}

Action* FormField::activationAction() const
{
    Q_D( const FormField );
    return d->m_activateAction;
}

void FormField::setActivationAction( Action *action )
{
    Q_D( FormField );
    delete d->m_activateAction;
    d->m_activateAction = action;
}

Action* FormField::additionalAction( AdditionalActionType type ) const
{
    Q_D( const FormField );
    return d->m_additionalActions.value(type);
}

void FormField::setAdditionalAction( AdditionalActionType type, Action *action )
{
    Q_D( FormField );
    delete d->m_additionalActions.value(type);
    d->m_additionalActions[type] = action;
}

Action* FormField::additionalAction( Annotation::AdditionalActionType type ) const
{
    Q_D( const FormField );
    return d->m_additionalAnnotActions.value(type);
}

void FormField::setAdditionalAction( Annotation::AdditionalActionType type, Action *action )
{
    Q_D( FormField );
    delete d->m_additionalAnnotActions.value(type);
    d->m_additionalAnnotActions[type] = action;
}


class Okular::FormFieldButtonPrivate : public Okular::FormFieldPrivate
{
    public:
        FormFieldButtonPrivate()
            : FormFieldPrivate( FormField::FormButton )
        {
        }

        Q_DECLARE_PUBLIC( FormFieldButton )

        void setValue( const QString& v ) override
        {
            Q_Q( FormFieldButton );
            q->setState( QVariant( v ).toBool() );
        }

        QString value() const override
        {
            Q_Q( const FormFieldButton );
            return qVariantFromValue<bool>( q->state() ).toString();
        }
};


FormFieldButton::FormFieldButton()
    : FormField( *new FormFieldButtonPrivate() )
{
}

FormFieldButton::~FormFieldButton()
{
}

void FormFieldButton::setState( bool )
{
}


class Okular::FormFieldTextPrivate : public Okular::FormFieldPrivate
{
    public:
        FormFieldTextPrivate()
            : FormFieldPrivate( FormField::FormText )
        {
        }

        Q_DECLARE_PUBLIC( FormFieldText )

        void setValue( const QString& v ) override
        {
            Q_Q( FormFieldText );
            q->setText( v );
        }

        QString value() const override
        {
            Q_Q( const FormFieldText );
            return q->text();
        }
};


FormFieldText::FormFieldText()
    : FormField( *new FormFieldTextPrivate() )
{
}

FormFieldText::~FormFieldText()
{
}

void FormFieldText::setText( const QString& )
{
}

bool FormFieldText::isPassword() const
{
    return false;
}

bool FormFieldText::isRichText() const
{
    return false;
}

int FormFieldText::maximumLength() const
{
    return -1;
}

Qt::Alignment FormFieldText::textAlignment() const
{
    return Qt::AlignVCenter | Qt::AlignLeft;
}

bool FormFieldText::canBeSpellChecked() const
{
    return false;
}


class Okular::FormFieldChoicePrivate : public Okular::FormFieldPrivate
{
    public:
        FormFieldChoicePrivate()
            : FormFieldPrivate( FormField::FormChoice )
        {
        }

        Q_DECLARE_PUBLIC( FormFieldChoice )

        void setValue( const QString& v ) override
        {
            Q_Q( FormFieldChoice );
            QStringList choices = v.split( QLatin1Char (';'), QString::SkipEmptyParts );
            QList<int> newchoices;
            foreach ( const QString& str, choices )
            {
                bool ok = true;
                int val = str.toInt( &ok );
                if ( ok )
                    newchoices.append( val );
            }
            if ( !newchoices.isEmpty() )
                q->setCurrentChoices( newchoices );
        }

        QString value() const override
        {
            Q_Q( const FormFieldChoice );
            QList<int> choices = q->currentChoices();
            std::sort(choices.begin(), choices.end());
            QStringList list;
            foreach ( int c, choices )
            {
                list.append( QString::number( c ) );
            }
            return list.join( QStringLiteral( ";" ) );
        }
};


FormFieldChoice::FormFieldChoice()
    : FormField( *new FormFieldChoicePrivate() )
{
}

FormFieldChoice::~FormFieldChoice()
{
}

bool FormFieldChoice::isEditable() const
{
    return false;
}

bool FormFieldChoice::multiSelect() const
{
    return false;
}

void FormFieldChoice::setCurrentChoices( const QList< int >& )
{
}

QString FormFieldChoice::editChoice() const
{
    return QString();
}

void FormFieldChoice::setEditChoice( const QString& )
{
}

Qt::Alignment FormFieldChoice::textAlignment() const
{
    return Qt::AlignVCenter | Qt::AlignLeft;
}

bool FormFieldChoice::canBeSpellChecked() const
{
    return false;
}

class Okular::FormFieldSignaturePrivate : public Okular::FormFieldPrivate
{
    public:
        FormFieldSignaturePrivate()
            : FormFieldPrivate( FormField::FormSignature )
        {
        }

        Q_DECLARE_PUBLIC( FormFieldSignature )

        void setValue( const QString& v ) override
        {
            Q_UNUSED( v )
        }

        QString value() const override
        {
            return QString();
        }
};

FormFieldSignature::FormFieldSignature()
    : FormField( *new FormFieldSignaturePrivate() )
{
}

FormFieldSignature::~FormFieldSignature()
{
}
