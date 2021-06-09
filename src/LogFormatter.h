#pragma once

#include "Core.h"

namespace plog
{

	/**
	 * \brief Custom formatter for plog.
	 */
	class LogFormatter
	{
	public:

		/**
		 * \brief Header formatter.
		 */
		static util::nstring header();

		/**
		 * \brief Record formatter.
		 */
		static util::nstring format(const Record& record);
	};

}//namespace plog