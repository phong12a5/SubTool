/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "about"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "GoogleRouteBase.h"
#include "gdrive/about/AboutAboutResource.h"

namespace googleQt{
namespace about{


    class AboutRoutes: public GoogleRouteBase{
    public:
        AboutRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('get')


            Gets information about the user, the user's Drive, and system
            capabilities.

            */
        std::unique_ptr<AboutResource> get(const gdrive::AboutArg& arg);
        GoogleTask<AboutResource>* get_Async(const gdrive::AboutArg& arg);
        void get_AsyncCB(
            const gdrive::AboutArg&,
            std::function<void(std::unique_ptr<AboutResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//AboutRoutes

}//about
}//googleQt
