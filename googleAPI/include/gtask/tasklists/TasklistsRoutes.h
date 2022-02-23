/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasklists"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gtask/GtaskRequestArg.h"
#include "GoogleRouteBase.h"
#include "gtask/tasklists/TasklistsTaskListCollectionRes.h"
#include "gtask/tasklists/TasklistsTaskListResource.h"

namespace googleQt{
namespace tasklists{


    class TasklistsRoutes: public GoogleRouteBase{
    public:
        TasklistsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('delete')


            Updates the authenticated user's specified task list

            */
        void deleteOperation(const gtask::TaskListContainerIdArg& );
        GoogleVoidTask* deleteOperation_Async(const gtask::TaskListContainerIdArg& arg);
        void deleteOperation_AsyncCB(
            const gtask::TaskListContainerIdArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('get')


            Returns the specified tasklist.

            */
        std::unique_ptr<TaskListResource> get(const gtask::TaskListContainerIdArg& arg);
        GoogleTask<TaskListResource>* get_Async(const gtask::TaskListContainerIdArg& arg);
        void get_AsyncCB(
            const gtask::TaskListContainerIdArg&,
            std::function<void(std::unique_ptr<TaskListResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('insert')


            Creates a new task list and adds it to the authenticated user's task
            lists

            */
        std::unique_ptr<TaskListResource> insert(const TaskListResource& body);
        GoogleTask<TaskListResource>* insert_Async(const TaskListResource& body);
        void insert_AsyncCB(
            const TaskListResource& body,
            std::function<void(std::unique_ptr<TaskListResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Returns all the authenticated user's task lists.

            */
        std::unique_ptr<TaskListCollectionRes> list(const gtask::TasklistsListArg& arg);
        GoogleTask<TaskListCollectionRes>* list_Async(const gtask::TasklistsListArg& arg);
        void list_AsyncCB(
            const gtask::TasklistsListArg&,
            std::function<void(std::unique_ptr<TaskListCollectionRes>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('update')


            Updates the authenticated user's specified task list

            */
        std::unique_ptr<TaskListResource> update(const gtask::TaskListContainerIdArg& arg, const TaskListResource& body);
        GoogleTask<TaskListResource>* update_Async(const gtask::TaskListContainerIdArg& arg, const TaskListResource& body);
        void update_AsyncCB(
            const gtask::TaskListContainerIdArg&,
            const TaskListResource& body,
            std::function<void(std::unique_ptr<TaskListResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//TasklistsRoutes

}//tasklists
}//googleQt
