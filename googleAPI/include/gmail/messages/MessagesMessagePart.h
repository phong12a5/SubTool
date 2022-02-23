/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/messages/MessagesMessagePayloadHeader.h"
#include "gmail/messages/MessagesMessagePartBody.h"
#include "gmail/messages/MessagesMessagePart.h"

namespace googleQt{
namespace messages{
    class MessagePart{
        /**
            field: part_id: ID of the part
            field: mime_type: mime type: text/plain, text/html
            field: filename: file name
            field: headers: list of headers in the part
            field: body: body of the message
            field: parts: The list of subparts in the part
        */

    public:
        MessagePart(){};

        MessagePart(const QString& arg){ m_partId = arg; };
        virtual ~MessagePart(){};

    public:
            /**
                ID of the part
            */
        QString partid()const{return m_partId;};
        MessagePart& setPartid(const QString& arg){m_partId=arg;return *this;};

            /**
                mime type: text/plain, text/html
            */
        QString mimetype()const{return m_mimeType;};
        MessagePart& setMimetype(const QString& arg){m_mimeType=arg;return *this;};

            /**
                file name
            */
        QString filename()const{return m_filename;};
        MessagePart& setFilename(const QString& arg){m_filename=arg;return *this;};

            /**
                list of headers in the part
            */
        const std::vector<MessagePayloadHeader>& headers()const{return m_headers;};
        MessagePart& setHeaders(const std::vector<MessagePayloadHeader>& arg){m_headers=arg;return *this;};

            /**
                body of the message
            */
        const MessagePartBody& body()const{return m_body;};
        MessagePart& setBody(const MessagePartBody& arg){m_body=arg;return *this;};

            /**
                The list of subparts in the part
            */
        const std::vector<MessagePart>& parts()const{return m_parts;};
        MessagePart& setParts(const std::vector<MessagePart>& arg){m_parts=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<MessagePart>  create(const QByteArray& data);
            static std::unique_ptr<MessagePart>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<MessagePart> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                ID of the part
            */
        QString m_partId;

            /**
                mime type: text/plain, text/html
            */
        QString m_mimeType;

            /**
                file name
            */
        QString m_filename;

            /**
                list of headers in the part
            */
        std::vector<MessagePayloadHeader> m_headers;

            /**
                body of the message
            */
        MessagePartBody m_body;

            /**
                The list of subparts in the part
            */
        std::vector<MessagePart> m_parts;

    };//MessagePart

}//messages
}//googleQt
