//
// Created by Léo KRYS on 21/04/2025.
//

#include "Decorator/AttachmentsDecorator.h"
#include <sstream>
#include <algorithm>
#include <ctime>

AttachmentsDecorator::AttachmentsDecorator(std::unique_ptr<Intervention> intervention)
    : InterventionDecorator(std::move(intervention)) {
}

bool AttachmentsDecorator::addAttachment(const std::string& filename, const std::string& description) {
    // Check if the file is already attached
    if (hasAttachment(filename)) {
        return false;
    }

    // Add the new attachment
    attachments.emplace_back(filename, description, std::time(nullptr));
    return true;
}

bool AttachmentsDecorator::removeAttachment(const std::string& filename) {
    auto it = std::find_if(attachments.begin(), attachments.end(),
                          [&filename](const Attachment& attachment) {
                              return attachment.filename == filename;
                          });

    if (it == attachments.end()) {
        return false; // File not found
    }

    attachments.erase(it);
    return true;
}

std::vector<std::string> AttachmentsDecorator::getAttachmentFilenames() const {
    std::vector<std::string> filenames;
    filenames.reserve(attachments.size());

    for (const auto& attachment : attachments) {
        filenames.push_back(attachment.filename);
    }

    return filenames;
}

bool AttachmentsDecorator::hasAttachment(const std::string& filename) const {
    return std::any_of(attachments.begin(), attachments.end(),
                      [&filename](const Attachment& attachment) {
                          return attachment.filename == filename;
                      });
}

size_t AttachmentsDecorator::getAttachmentCount() const {
    return attachments.size();
}

std::string AttachmentsDecorator::getInfo() const {
    // Get the base info from the wrapped intervention
    std::string baseInfo = InterventionDecorator::getInfo();

    std::ostringstream attachInfo;
    attachInfo << baseInfo << std::endl << "Attachments: " << attachments.size();

    // List all attachments if any
    if (!attachments.empty()) {
        attachInfo << std::endl << "Files:";
        for (const auto& attachment : attachments) {
            // Format the timestamp
            char timeBuffer[25];
            std::strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y %H:%M:%S",
                         std::localtime(&attachment.uploadTime));

            attachInfo << std::endl << "- " << attachment.filename;

            if (!attachment.description.empty()) {
                attachInfo << " (" << attachment.description << ")";
            }

            attachInfo << " [Added: " << timeBuffer << "]";
        }
    }

    return attachInfo.str();
}