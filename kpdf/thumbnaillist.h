/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef THUMBNAILLIST_H
#define THUMBNAILLIST_H

#include <qscrollview.h>
#include <qvaluevector.h>
#include <qvbox.h>
#include "document.h"

class QTimer;
class KActionCollection;
class KConfigGroup;
class ThumbnailWidget;

/**
 * @short A scrollview that displays pages pixmaps previews (aka thumbnails).
 *
 * ...
 */
class ThumbnailList : public QScrollView, public KPDFDocumentObserver
{
Q_OBJECT
	public:
		ThumbnailList(QWidget *parent, KPDFDocument *document);

		// create actions that interact with this widget and load/save settings
		uint observerId() const { return THUMBNAILS_ID; }
		void setupActions( KActionCollection * /*collection*/, KConfigGroup * /*config*/ ) {};
		void saveSettings( KConfigGroup * /*config*/ ) {};

		// create thumbnails ( inherited as a DocumentObserver )
		void pageSetup( const QValueVector<KPDFPage*> & pages, bool documentChanged );

		// hilihght current thumbnail ( inherited as DocumentObserver )
		void pageSetCurrent( int pageNumber, const QRect & viewport );

		// redraw thumbnail ( inherited as DocumentObserver )
		void notifyPixmapChanged( int pageNumber );

	protected:
		// scroll up/down the view
		void keyPressEvent( QKeyEvent * e );

		// select a thumbnail by clicking on it
		void contentsMousePressEvent( QMouseEvent * );

		// resize thumbnails to fit the width
		void viewportResizeEvent( QResizeEvent * );

		// file drop related events (an url may be dropped even here)
		void dragEnterEvent( QDragEnterEvent* );
		void dropEvent( QDropEvent* );

	signals:
		void urlDropped( const KURL& );

	public slots:
		// make requests for generating pixmaps for visible thumbnails
		void slotRequestPixmaps( int newContentsX = -1, int newContentsY = -1 );

	private:
		void requestPixmaps( int delayMs = 0 );
		KPDFDocument *m_document;
		ThumbnailWidget *m_selected;
		QTimer *m_delayTimer;
		QValueVector<ThumbnailWidget *> m_thumbnails;
		int m_vectorIndex;
};

/**
 * @short A vertical boxed container with zero size hint (for insertion on left toolbox)
 */
class ThumbnailsBox : public QVBox
{
	public:
		ThumbnailsBox( QWidget * parent ) : QVBox( parent ) {};
		QSize sizeHint() const { return QSize(); }
};

#endif
