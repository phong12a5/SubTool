/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "GoogleRouteBase.h"
#include "gdrive/files/FilesFileResource.h"
#include "gdrive/files/FilesFileResourcesCollection.h"
#include "gdrive/files/FilesGeneratedIdsCollection.h"

namespace googleQt{
namespace files{


    class FilesRoutes: public GoogleRouteBase{
    public:
        FilesRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('copy')


            Creates a copy of a file and applies any requested updates with
            patch semantics.

            */
        std::unique_ptr<FileResource> copy(const gdrive::CopyFileArg& arg, const FileResource& body);
        GoogleTask<FileResource>* copy_Async(const gdrive::CopyFileArg& arg, const FileResource& body);
        void copy_AsyncCB(
            const gdrive::CopyFileArg&,
            const FileResource& body,
            std::function<void(std::unique_ptr<FileResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('create')


            Creates a new file or folder.

            */
        std::unique_ptr<FileResource> create(const gdrive::CreateFileArg& arg, QIODevice* readFrom);
        GoogleTask<FileResource>* create_Async(const gdrive::CreateFileArg& arg, QIODevice* data);
        void create_AsyncCB(
            const gdrive::CreateFileArg&,
            QIODevice* data,
            std::function<void(std::unique_ptr<FileResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('createEmptyFile')


            Creates a new empty file providing metadata but to content.

            */
        std::unique_ptr<FileResource> createEmptyFile(const gdrive::CreateFileArg& arg);
        GoogleTask<FileResource>* createEmptyFile_Async(const gdrive::CreateFileArg& arg);
        void createEmptyFile_AsyncCB(
            const gdrive::CreateFileArg&,
            std::function<void(std::unique_ptr<FileResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('createFolder')


            Creates a new folder.

            */
        std::unique_ptr<FileResource> createFolder(const gdrive::CreateFolderArg& arg);
        GoogleTask<FileResource>* createFolder_Async(const gdrive::CreateFolderArg& arg);
        void createFolder_AsyncCB(
            const gdrive::CreateFolderArg&,
            std::function<void(std::unique_ptr<FileResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('delete')


            Permanently deletes a file owned by the user without moving it to
            the trash. If the target is a folder, all descendants owned by the
            user are also deleted.

            */
        void deleteOperation(const gdrive::DeleteFileArg& );
        GoogleVoidTask* deleteOperation_Async(const gdrive::DeleteFileArg& arg);
        void deleteOperation_AsyncCB(
            const gdrive::DeleteFileArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('downloadFile')


            Get file media content.

            */
        void downloadFile(const gdrive::DownloadFileArg& , QIODevice* writeTo);
        GoogleVoidTask* downloadFile_Async(const gdrive::DownloadFileArg& arg, QIODevice* data);
        void downloadFile_AsyncCB(
            const gdrive::DownloadFileArg&,
            QIODevice* data,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('emptyTrash')


            Permanently deletes a file owned by the user without moving it to
            the trash. If the target is a folder, all descendants owned by the
            user are also deleted.

            */
        void emptyTrash(const gdrive::EmptyTrashArg& );
        GoogleVoidTask* emptyTrash_Async(const gdrive::EmptyTrashArg& arg);
        void emptyTrash_AsyncCB(
            const gdrive::EmptyTrashArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('generateIds')


            Generates a set of file IDs which can be provided in create
            requests.

            */
        std::unique_ptr<GeneratedIdsCollection> generateIds(const gdrive::GenerateIdArg& arg);
        GoogleTask<GeneratedIdsCollection>* generateIds_Async(const gdrive::GenerateIdArg& arg);
        void generateIds_AsyncCB(
            const gdrive::GenerateIdArg&,
            std::function<void(std::unique_ptr<GeneratedIdsCollection>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('get')


            Get file metadata.

            */
        std::unique_ptr<FileResource> get(const gdrive::GetFileArg& arg);
        GoogleTask<FileResource>* get_Async(const gdrive::GetFileArg& arg);
        void get_AsyncCB(
            const gdrive::GetFileArg&,
            std::function<void(std::unique_ptr<FileResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Lists or searches files.

            */
        std::unique_ptr<FileResourcesCollection> list(const gdrive::FileListArg& arg);
        GoogleTask<FileResourcesCollection>* list_Async(const gdrive::FileListArg& arg);
        void list_AsyncCB(
            const gdrive::FileListArg&,
            std::function<void(std::unique_ptr<FileResourcesCollection>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('moveFile')


            Move file.

            */
        std::unique_ptr<FileResource> moveFile(const gdrive::MoveFileArg& arg);
        GoogleTask<FileResource>* moveFile_Async(const gdrive::MoveFileArg& arg);
        void moveFile_AsyncCB(
            const gdrive::MoveFileArg&,
            std::function<void(std::unique_ptr<FileResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('rename')


            Rename file.

            */
        std::unique_ptr<FileResource> rename(const gdrive::RenameFileArg& arg);
        GoogleTask<FileResource>* rename_Async(const gdrive::RenameFileArg& arg);
        void rename_AsyncCB(
            const gdrive::RenameFileArg&,
            std::function<void(std::unique_ptr<FileResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('updateFileMeta')


            Update file meta.

            */
        std::unique_ptr<FileResource> updateFileMeta(const gdrive::UpdateFileArg& arg);
        GoogleTask<FileResource>* updateFileMeta_Async(const gdrive::UpdateFileArg& arg);
        void updateFileMeta_AsyncCB(
            const gdrive::UpdateFileArg&,
            std::function<void(std::unique_ptr<FileResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('uploadFileSimple')


            Upload file content.

            */
        std::unique_ptr<FileResource> uploadFileSimple(QIODevice* readFrom);
        GoogleTask<FileResource>* uploadFileSimple_Async(QIODevice* data);
        void uploadFileSimple_AsyncCB(
            QIODevice* data,
            std::function<void(std::unique_ptr<FileResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//FilesRoutes

}//files
}//googleQt
