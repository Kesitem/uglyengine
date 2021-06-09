#pragma once

#include "core.h"

namespace ugly
{
    /**
     * \brief Base application.
     */
    class Application
    {
        public:

            /**
             * \brief Constructor.
             */
            Application();

            /**
             * \brief Destructor.
             */
            virtual ~Application();

            /**
             * \brief Initialize application.
             * 
             * \return False if error
             */
            virtual bool initialize();

            /**
             * \brief Shutdown application.
             */
            virtual void shutdown();

            /**
             * \brief Update application.
             */
            virtual void update();

            /**
             * \brief Get application name.
             * 
             * \return Application name
             */
            const std::string &getName() const;

        protected:

            /*! Application name */
            std::string m_name {"UglyBaseApplication"};
    };

}//namespace ugly