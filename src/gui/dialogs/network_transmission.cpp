/* $Id$ */
/*
   Copyright (C) 2011 Sergey Popov <loonycyborg@gmail.com>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#define GETTEXT_DOMAIN "wesnoth-lib"

#include "gui/dialogs/network_transmission.hpp"

#include "foreach.hpp"
#include "formula_string_utils.hpp"
#include "gettext.hpp"
#include "gui/widgets/button.hpp"
#include "gui/widgets/progress_bar.hpp"
#include "gui/widgets/label.hpp"
#include "gui/widgets/settings.hpp"
#include "gui/widgets/window.hpp"
#include "log.hpp"

namespace gui2 {

REGISTER_DIALOG(network_transmission)

void tnetwork_transmission::pump_monitor::process(events::pump_info&)
{
	connection_.poll();
	if(!window_) return;
	if(connection_.done()) {
		window_.get().set_retval(twindow::OK);
	} else {
		if(connection_.bytes_to_read()) {
			size_t completed = connection_.bytes_read();
			size_t total = connection_.bytes_to_read();
			find_widget<tprogress_bar>(&(window_.get()), "progress", false)
				.set_percentage((completed*100)/total);

			string_map symbols;
			symbols["total"] = str_cast(total/1024);
			symbols["completed"] = str_cast(completed/1024);

			find_widget<tlabel>(&(window_.get()), "numeric_progress", false)
					.set_label(vgettext(
						  "$completed KiB/$total KiB"
						, symbols));
			window_->invalidate_layout();
		}
	}
}

tnetwork_transmission::tnetwork_transmission(
		  network_asio::connection& connection
		, const std::string& title
		, const std::string& subtitle)
	: connection_(connection)
	, pump_monitor(connection)
	, subtitle_(subtitle)
{
	register_label("title", true, title, false);
}

void tnetwork_transmission::set_subtitle(const std::string& subtitle)
{
	subtitle_ = subtitle;
}

void tnetwork_transmission::pre_show(CVideo& /*video*/, twindow& window)
{
	// ***** ***** ***** ***** Set up the widgets ***** ***** ***** *****
	if(!subtitle_.empty()) {
		find_widget<tlabel>(&window, "subtitle", false).set_label(subtitle_);
	}

	pump_monitor.window_ = window;

}

void tnetwork_transmission::post_show(twindow& /*window*/)
{
	pump_monitor.window_.reset();
	connection_.cancel();
}

} // namespace gui2

