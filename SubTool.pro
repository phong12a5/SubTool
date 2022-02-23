#QT +=   quick
QT += widgets
QT += core gui qml quick
QT += sql xml
QT += network

#CONFIG += c++11 console
CONFIG += c++11
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS += \
    Dialog/infoversion.ui \
    Dialog/processdownload.ui \
    Dialog/tokendialog.ui \
    Dialog/viewlog.ui \
    UI/Config/checkpoint.ui \
    UI/Config/cloneconfig.ui \
    UI/Config/interactconfig.ui \
    UI/Config/proxyconfig.ui \
    UI/Config/statusclone.ui \
    UI/Config/subwindow.ui \
    UI/Config/viewclone.ui \
    UI/mainwindow.ui \
    mainwindow_bak.ui \

SOURCES += \
        Dialog/infoversion.cpp \
        Dialog/processdownload.cpp \
        Dialog/tokendialog.cpp \
        Dialog/viewlog.cpp \
        UI/Config/checkpoint.cpp \
        UI/Config/cloneconfig.cpp \
        UI/Config/interactconfig.cpp \
        UI/Config/proxyconfig.cpp \
        UI/Config/statusclone.cpp \
        UI/Config/subwindow.cpp \
        UI/Config/viewclone.cpp \
        UI/mainwindow.cpp \
        controllermainthread.cpp \
        controllerworkerthread.cpp \
        fcareapplication.cpp \
        log.cpp \
        main.cpp \
        appmain.cpp \
        model/CommonInforClone.cpp \
        model/servicedata.cpp \
        qaesencryption/qaesencryption.cpp \
        service/baseservice.cpp \
        service/chromeservice.cpp \
        service/servicemanager.cpp \
        supporter/PhoneNumberAPI.cpp \
        supporter/captchaapi.cpp \
        supporter/followpageapi.cpp \
        supporter/networkaccesscontrollersupporter.cpp \
        supporter/networkaccesssupporter.cpp \
        supporter/workerthread.cpp \
        utils.cpp \
        worker/baseworker.cpp \
        worker/chromeworker.cpp \
        worker/downloadmanager.cpp \
        worker/sqliteworker.cpp
HEADERS += \
    AppDefine.h \
    AppEnum.h \
    DefineString.h \
    Dialog/infoversion.h \
    Dialog/processdownload.h \
    Dialog/tokendialog.h \
    Dialog/tokendialog.h \
    Dialog/viewlog.h \
    UI/Config/checkpoint.h \
    UI/Config/cloneconfig.h \
    UI/Config/interactconfig.h \
    UI/Config/proxyconfig.h \
    UI/Config/statusclone.h \
    UI/Config/subwindow.h \
    UI/Config/viewclone.h \
    UI/definition.h \
    UI/mainwindow.h \
    appmain.h \
    controllermainthread.h \
    controllerworkerthread.h \
    fcareapplication.h \
    googleAPI/include/Endpoint.h \
    googleAPI/include/GoogleClient.h \
    googleAPI/include/GoogleRouteBase.h \
    googleAPI/include/gcontact/GcontactCache.h \
    googleAPI/include/gcontact/GcontactCacheUtil.h \
    googleAPI/include/gcontact/GcontactParts.h \
    googleAPI/include/gcontact/GcontactRequestArg.h \
    googleAPI/include/gcontact/GcontactRoutes.h \
    googleAPI/include/gcontact/contact_group/ContactGroupRoutes.h \
    googleAPI/include/gcontact/contacts/ContactsRoutes.h \
    googleAPI/include/gdrive/GdriveRequestArg.h \
    googleAPI/include/gdrive/GdriveRoutes.h \
    googleAPI/include/gdrive/about/AboutAboutResource.h \
    googleAPI/include/gdrive/about/AboutRoutes.h \
    googleAPI/include/gdrive/about/AboutStorageQuota.h \
    googleAPI/include/gdrive/about/AboutUserInfo.h \
    googleAPI/include/gdrive/comments/CommentsComment.h \
    googleAPI/include/gdrive/comments/CommentsCommentListResult.h \
    googleAPI/include/gdrive/comments/CommentsQuotedFileContent.h \
    googleAPI/include/gdrive/comments/CommentsReply.h \
    googleAPI/include/gdrive/comments/CommentsRoutes.h \
    googleAPI/include/gdrive/comments/CommentsUser.h \
    googleAPI/include/gdrive/files/FilesContentHints.h \
    googleAPI/include/gdrive/files/FilesCreateFileDetails.h \
    googleAPI/include/gdrive/files/FilesFileResource.h \
    googleAPI/include/gdrive/files/FilesFileResourcesCollection.h \
    googleAPI/include/gdrive/files/FilesGeneratedIdsCollection.h \
    googleAPI/include/gdrive/files/FilesImageMediaMetadata.h \
    googleAPI/include/gdrive/files/FilesLocationData.h \
    googleAPI/include/gdrive/files/FilesRoutes.h \
    googleAPI/include/gdrive/files/FilesThumbnailInfo.h \
    googleAPI/include/gdrive/files/FilesUpdateFileDetails.h \
    googleAPI/include/gdrive/files/FilesVideoMediaMetadata.h \
    googleAPI/include/gdrive/permissions/PermissionsPermissionResourcesCollection.h \
    googleAPI/include/gdrive/permissions/PermissionsResourcePermission.h \
    googleAPI/include/gdrive/permissions/PermissionsRoutes.h \
    googleAPI/include/gdrive/revisions/RevisionsRevisionResource.h \
    googleAPI/include/gdrive/revisions/RevisionsRevisionResourceCollection.h \
    googleAPI/include/gdrive/revisions/RevisionsRevisionUser.h \
    googleAPI/include/gdrive/revisions/RevisionsRoutes.h \
    googleAPI/include/gdrive/revisions/RevisionsUpdateRevisionDetails.h \
    googleAPI/include/gmail/GmailCache.h \
    googleAPI/include/gmail/GmailCacheRoutes.h \
    googleAPI/include/gmail/GmailRequestArg.h \
    googleAPI/include/gmail/GmailRoutes.h \
    googleAPI/include/gmail/attachments/AttachmentsMessageAttachment.h \
    googleAPI/include/gmail/attachments/AttachmentsRoutes.h \
    googleAPI/include/gmail/drafts/DraftsDraftListRes.h \
    googleAPI/include/gmail/drafts/DraftsDraftResource.h \
    googleAPI/include/gmail/drafts/DraftsRoutes.h \
    googleAPI/include/gmail/errors/ErrorsErrorInfo.h \
    googleAPI/include/gmail/errors/ErrorsErrorPart.h \
    googleAPI/include/gmail/history/HistoryHistoryRecord.h \
    googleAPI/include/gmail/history/HistoryHistoryRecordList.h \
    googleAPI/include/gmail/history/HistoryLabelHistoryRecord.h \
    googleAPI/include/gmail/history/HistoryRoutes.h \
    googleAPI/include/gmail/labels/LabelsLabelResource.h \
    googleAPI/include/gmail/labels/LabelsLabelsResultList.h \
    googleAPI/include/gmail/labels/LabelsRoutes.h \
    googleAPI/include/gmail/messages/MessagesBatchDeleteBodyArg.h \
    googleAPI/include/gmail/messages/MessagesMessageListRes.h \
    googleAPI/include/gmail/messages/MessagesMessageMimeBody.h \
    googleAPI/include/gmail/messages/MessagesMessagePart.h \
    googleAPI/include/gmail/messages/MessagesMessagePartBody.h \
    googleAPI/include/gmail/messages/MessagesMessagePayload.h \
    googleAPI/include/gmail/messages/MessagesMessagePayloadHeader.h \
    googleAPI/include/gmail/messages/MessagesMessageResource.h \
    googleAPI/include/gmail/messages/MessagesRoutes.h \
    googleAPI/include/gmail/threads/ThreadsRoutes.h \
    googleAPI/include/gmail/threads/ThreadsThreadListRes.h \
    googleAPI/include/gmail/threads/ThreadsThreadResource.h \
    googleAPI/include/gmail/users/UsersGetProfileResponse.h \
    googleAPI/include/gmail/users/UsersRoutes.h \
    googleAPI/include/google/demo/ApiListener.h \
    googleAPI/include/google/demo/ApiTerminal.h \
    googleAPI/include/google/endpoint/ApiAppInfo.h \
    googleAPI/include/google/endpoint/ApiAuthInfo.h \
    googleAPI/include/google/endpoint/ApiAutotest.h \
    googleAPI/include/google/endpoint/ApiBase.h \
    googleAPI/include/google/endpoint/ApiClient.h \
    googleAPI/include/google/endpoint/ApiEndpoint.h \
    googleAPI/include/google/endpoint/ApiException.h \
    googleAPI/include/google/endpoint/ApiUtil.h \
    googleAPI/include/google/endpoint/BatchRunner.h \
    googleAPI/include/google/endpoint/Cache.h \
    googleAPI/include/google/endpoint/GoogleHost.h \
    googleAPI/include/google/endpoint/GoogleTask.h \
    googleAPI/include/google/endpoint/GoogleWebAuth.h \
    googleAPI/include/gtask/GtaskCache.h \
    googleAPI/include/gtask/GtaskRequestArg.h \
    googleAPI/include/gtask/GtaskRoutes.h \
    googleAPI/include/gtask/tasklists/TasklistsRoutes.h \
    googleAPI/include/gtask/tasklists/TasklistsTaskListCollectionRes.h \
    googleAPI/include/gtask/tasklists/TasklistsTaskListResource.h \
    googleAPI/include/gtask/tasks/TasksRoutes.h \
    googleAPI/include/gtask/tasks/TasksTaskCollectionRes.h \
    googleAPI/include/gtask/tasks/TasksTaskLink.h \
    googleAPI/include/gtask/tasks/TasksTaskResource.h \
    log.h \
    model/CommonInforClone.h \
    model/servicedata.h \
    qaesencryption/aesni/aesni-enc-cbc.h \
    qaesencryption/aesni/aesni-enc-ecb.h \
    qaesencryption/aesni/aesni-key-exp.h \
    qaesencryption/qaesencryption.h \
    service/baseservice.h \
    service/chromeservice.h \
    service/servicemanager.h \
    fdriver/include/fdriver_global.h \
    supporter/PhoneNumberAPI.h \
    supporter/captchaapi.h \
    supporter/followpageapi.h \
    supporter/networkaccesscontrollersupporter.h \
    supporter/networkaccesssupporter.h \
    supporter/workerthread.h \
    utils.h \
    worker/baseworker.h \
    worker/chromeworker.h \
    worker/downloadmanager.h \
    worker/sqliteworker.h

SOURCES += \
    mainwindow_bak.cpp

HEADERS += \
    mainwindow_bak.h

INCLUDEPATH += $$PWD/curl/include $$PWD/worker + $$PWD/curl/include $$PWD/model + $$PWD/curl/include $$PWD/service + $$PWD/curl/include $$PWD/UI + $$PWD/curl/include $$PWD/supporter + $$PWD/curl/include $$PWD/Dialog
INCLUDEPATH += $$PWD/qaesencryption
INCLUDEPATH += $$PWD/fdriver/include
#INCLUDEPATH += $$PWD/fity.one-api/fcare/include
INCLUDEPATH += $$PWD/googleAPI/include
INCLUDEPATH += $$PWD/quazip/include
INCLUDEPATH += $$PWD/autoFarmerAPI/include

win32: {
    LIBS += -L$$PWD/curl/libs -llibcurl -llibcurl_imp

     CONFIG(release, debug|release) {
        LIBS += -L$$PWD/fdriver/libs/release -lfdriver
        #LIBS += -L$$PWD/fity.one-api/fcare/libs/release/ -lfcare.fityone
        LIBS += -L$$PWD/googleAPI/release/ -llibgoogleQt
        LIBS += -L$$PWD/quazip/release/ -llibquazip
        LIBS += -L$$PWD/autoFarmerAPI/libs/release/ -lWebAPI
    }

    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/fdriver/libs/debug -lfdriver
        #LIBS += -L$$PWD/fity.one-api/fcare/libs/debug/ -lfcare.fityone
        LIBS += -L$$PWD/googleAPI/debug/ -llibgoogleQt
        LIBS += -L$$PWD/quazip/debug/ -llibquazipd
        LIBS += -L$$PWD/autoFarmerAPI/libs/debug/ -lWebAPI
        DEFINES += __DEBUG_MODE__
    }

    #copy chromedriver.exe to output folder
    !exists( $$OUT_PWD/chromedriver.exe) {
        QMAKE_POST_LINK += copy /y "$$shell_path($$PWD/chromedriver.exe)" "$$shell_path($$OUT_PWD)"
    }
}

RC_ICONS = Logo.ico



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    UI/Resources/icons8_add_30px.png \
    UI/Resources/icons8_stop_1.ico \
    UI/Resources/icons8_sync.png \
    UI/Resources/round-play-button.png \
    db_maxcare.sqlite \
    model/userAgent.json

