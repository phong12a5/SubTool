/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "permissions"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "gdrive/permissions/PermissionsResourcePermission.h"

namespace googleQt{
namespace permissions{
    class PermissionResourcesCollection{
        /**
            field: kind: This is always drive#permissionList.
            field: permissions: The full list of permissions.
        */

    public:
        PermissionResourcesCollection():
        m_kind("drive#permissionList")
        {};

        PermissionResourcesCollection(const QString& arg):
        m_kind("drive#permissionList")
        { m_kind = arg; };
        virtual ~PermissionResourcesCollection(){};

    public:
            /**
                This is always drive#permissionList.
            */
        QString kind()const{return m_kind;};
        PermissionResourcesCollection& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                The full list of permissions.
            */
        const std::vector<ResourcePermission>& permissions()const{return m_permissions;};
        PermissionResourcesCollection& setPermissions(const std::vector<ResourcePermission>& arg){m_permissions=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<PermissionResourcesCollection>  create(const QByteArray& data);
            static std::unique_ptr<PermissionResourcesCollection>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<PermissionResourcesCollection> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                This is always drive#permissionList.
            */
        QString m_kind;

            /**
                The full list of permissions.
            */
        std::vector<ResourcePermission> m_permissions;

    };//PermissionResourcesCollection

}//permissions
}//googleQt
