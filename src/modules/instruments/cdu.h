/* vim:ts=4
 *
 * Copyleft 2012…2015  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef XEFIS__MODULES__INSTRUMENTS__CDU_H__INCLUDED
#define XEFIS__MODULES__INSTRUMENTS__CDU_H__INCLUDED

// Standard:
#include <cstddef>
#include <vector>

// Qt:
#include <QtWidgets/QWidget>
#include <QtXml/QDomElement>

// Xefis:
#include <xefis/config/all.h>
#include <xefis/core/instrument.h>
#include <xefis/core/instrument_aids.h>
#include <xefis/core/property.h>


class CDU:
	public xf::Instrument,
	protected xf::InstrumentAids
{
  private:
	static constexpr double kButtonWidthForHeight = 0.9;

	enum class Column
	{
		Left,
		Right,
	};

	enum class ButtonState
	{
		Normal,
		Pressed,
		Disabled,
	};

	class Config;

	class Strip
	{
	  public:
		// Ctor
		explicit Strip (CDU&, QString const& title, Column);

		QString const&
		title() const noexcept;

		Column
		column() const noexcept;

		/**
		 * Assign rect.
		 */
		void
		set_rect (QRectF const&);

		/**
		 * Get the assigned rect.
		 */
		QRectF const&
		rect() const noexcept;

		/**
		 * Get CDU reference.
		 */
		CDU&
		cdu() const noexcept;

		/**
		 * Return true if any followed property is fresh.
		 */
		virtual bool
		fresh() const noexcept;

		virtual void
		handle_mouse_press (QMouseEvent*, CDU*);

		virtual void
		handle_mouse_release (QMouseEvent*, CDU*);

		virtual void
		paint (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool focused);

		virtual void
		paint_button (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool focused);

		virtual void
		paint_title (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool focused);

		virtual void
		paint_value (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool focused);

		virtual void
		paint_focus (QRectF const& rect, QRectF const& button_rect, xf::InstrumentAids&, xf::Painter&, Column);

		static void
		paint_button_helper (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, ButtonState);

		static void
		paint_title_helper (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, QString const& title, QColor);

		static void
		paint_value_helper (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, QString const& value, QColor);

		static void
		paint_focus_helper (QRectF const& rect, QRectF const& button_rect, xf::InstrumentAids&, xf::Painter&, Column);

	  private:
		CDU&	_cdu;
		QString	_title;
		Column	_column;
		QRectF	_rect;
	};

	class FillStrip: public Strip
	{
	  public:
		// Ctor
		FillStrip (CDU& cdu) noexcept:
			Strip (cdu, "", Column::Left)
		{ }
	};

	class EmptyStrip: public Strip
	{
	  public:
		// Ctor
		explicit EmptyStrip (CDU&, Column);

		// Strip
		void
		paint_button (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool focused) override;
	};

	class SettingStrip: public Strip
	{
	  public:
		// Ctor
		explicit SettingStrip (CDU&, QDomElement const& setting_element, Column);

		// Strip
		bool
		fresh() const noexcept override;

		// Strip
		void
		handle_mouse_press (QMouseEvent*, CDU*) override;

		// Strip
		void
		handle_mouse_release (QMouseEvent*, CDU*) override;

		// Strip
		void
		paint_button (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool focused) override;

		// Strip
		void
		paint_title (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool focused) override;

		// Strip
		void
		paint_value (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool focused) override;

		// Strip
		void
		paint_focus (QRectF const& rect, QRectF const& button_rect, xf::InstrumentAids&, xf::Painter&, Column) override;

	  private:
		xf::TypedProperty	_property;
		std::string			_format;
		std::string			_unit;
		std::string			_nil_value;
		std::string			_true_value;
		std::string			_false_value;
		ButtonState			_button_state	= ButtonState::Normal;
		bool				_read_only		= false;
		QRectF				_button_rect;
	};

	class GotoStrip: public Strip
	{
	  public:
		// Ctor
		explicit GotoStrip (CDU&, QDomElement const& goto_element, Column);

		QString const&
		target_page_id() const noexcept;

		// Strip
		void
		handle_mouse_press (QMouseEvent*, CDU*) override;

		// Strip
		void
		handle_mouse_release (QMouseEvent*, CDU*) override;

		// Strip
		void
		paint_button (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool focused) override;

		// Strip
		void
		paint_value (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool focused) override;

		// Strip
		void
		paint_focus (QRectF const& rect, QRectF const& button_rect, xf::InstrumentAids&, xf::Painter&, Column) override;

	  private:
		QString			_target_page_id;
		ButtonState		_button_state = ButtonState::Normal;
		QRectF			_button_rect;
	};

	class Page
	{
	  public:
		typedef std::vector<Unique<Strip>>	UniqueStrips;
		typedef std::vector<Strip*>			Strips;

	  public:
		// Ctor
		explicit Page (CDU&, QDomElement const& page_element, Config&, xf::Logger const&);

		/**
		 * Return unique page identifier.
		 */
		QString const&
		id() const noexcept;

		/**
		 * Return page title.
		 */
		QString const&
		title() const noexcept;

		/**
		 * Return strips list.
		 */
		UniqueStrips const&
		strips() const noexcept;

		/**
		 * Return strips list of the left panel.
		 */
		Strips const&
		strips_left() const noexcept;

		/**
		 * Return strips list of the right panel.
		 */
		Strips const&
		strips_right() const noexcept;

		/**
		 * Return true if any of the properties is fresh.
		 */
		bool
		scan_properties() const noexcept;

		/**
		 * Handle mouse move event.
		 * Return true if widget needs repaint.
		 */
		bool
		handle_mouse_move (QMouseEvent*);

		/**
		 * Handle mouse press event.
		 * Return true if widget needs repaint.
		 */
		bool
		handle_mouse_press (QMouseEvent*, CDU*);

		/**
		 * Handle mouse release event.
		 * Return true if widget needs repaint.
		 */
		bool
		handle_mouse_release (QMouseEvent*, CDU*);

		/**
		 * Paint the page.
		 */
		void
		paint (QRectF const& rect, xf::InstrumentAids&, xf::Painter&);

		/**
		 * Return px num from the left edge to the black box left edge.
		 */
		double
		bb_margin() const noexcept;

		/**
		 * Reset focused button, etc.
		 */
		void
		reset();

	  private:
		QString			_title;
		QString			_id;
		UniqueStrips	_strips;
		Strips			_strips_left;
		Strips			_strips_right;
		Strip*			_focused_strip	= nullptr;
		Strip*			_capture_strip	= nullptr;
		double			_bb_margin		= 0.0;
	};

	class Config
	{
	  public:
		typedef std::map<QString, Shared<Page>>	PagesMap;

	  public:
		// Ctor
		explicit Config (CDU& cdu, QDomElement const& pages_element, xf::Logger const&);

		/**
		 * Return true if any of the properties is fresh.
		 */
		bool
		scan_properties() const noexcept;

		/**
		 * Return default page ID.
		 */
		QString
		default_page_id() const noexcept;

		/**
		 * Return default page.
		 * May return nullptr.
		 */
		Page*
		default_page() const noexcept;

		/**
		 * Return number of button rows/strips per page.
		 */
		std::size_t
		rows() const noexcept;

		/**
		 * Return page by ID.
		 * May return nullptr.
		 */
		Page*
		find_page_by_id (QString const& id) const noexcept;

		/**
		 * Ensure that all pages are reachable from the first page.
		 * Log errors on logger.
		 */
		void
		check_reachability() const;

	  private:
		PagesMap			_pages_by_id;
		QString				_default_page_id;
		std::size_t			_rows = 8;
		xf::Logger const&	_logger;
	};

  public:
	// Ctor
	CDU (xf::ModuleManager*, QDomElement const& config);

	// Module
	void
	data_updated() override;

	/**
	 * Post message to the message board.
	 */
	void
	post_message (QString const&);

  protected:
	// QWidget
	void
	resizeEvent (QResizeEvent*) override;

	// QWidget
	void
	paintEvent (QPaintEvent*) override;

	// QWidget
	void
	keyPressEvent (QKeyEvent*) override;

	// QWidget
	void
	showEvent (QShowEvent*) override;

	// QWidget
	void
	mouseMoveEvent (QMouseEvent*) override;

	// QWidget
	void
	mousePressEvent (QMouseEvent*) override;

	// QWidget
	void
	mouseReleaseEvent (QMouseEvent*) override;

	/**
	 * Paint the black buttons box.
	 * And buttons of course.
	 */
	void
	paint_strips_area (QRectF const&);

	/**
	 * Paint entry area.
	 */
	void
	paint_entry_area (QRectF const&);

	/**
	 * Return current page.
	 * May return nullptr.
	 */
	Page*
	current_page() const noexcept;

	/**
	 * Switch current page. Do nothing if no such page exists.
	 */
	void
	switch_page (QString const& id);

	/**
	 * Return currently entered input text.
	 */
	QString
	entry_value() const;

	/**
	 * Clear input.
	 */
	void
	clear_entry_value();

  private:
	Unique<Config>			_config;
	QString					_current_page_id;
	xf::PropertyTime		_time_utc;
	QString					_entry_value;
	std::vector<QString>	_messages;
};


inline void
CDU::Strip::handle_mouse_press (QMouseEvent*, CDU*)
{ }


inline void
CDU::Strip::handle_mouse_release (QMouseEvent*, CDU*)
{ }


inline void
CDU::Strip::paint_button (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool)
{ }


inline void
CDU::Strip::paint_title (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool)
{ }


inline void
CDU::Strip::paint_value (QRectF const&, xf::InstrumentAids&, xf::Painter&, Column, bool)
{ }


inline void
CDU::Strip::paint_focus (QRectF const&, QRectF const&, xf::InstrumentAids&, xf::Painter&, Column)
{ }

#endif
