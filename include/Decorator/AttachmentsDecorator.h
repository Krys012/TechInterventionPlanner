//
// Created by Léo KRYS on 21/04/2025.
//

#ifndef ATTACHMENTS_DECORATOR_H
#define ATTACHMENTS_DECORATOR_H

#include "Decorator/InterventionDecorator.h"
#include <string>
#include <vector>

/**
 * @class AttachmentsDecorator
 * @brief Adds file attachment functionality to an intervention
 */
class AttachmentsDecorator : public InterventionDecorator {
private:
    struct Attachment {
        std::string filename;
        std::string description;
        std::time_t uploadTime;

        Attachment(const std::string& filename, const std::string& description,
                  std::time_t uploadTime = std::time(nullptr))
            : filename(filename), description(description), uploadTime(uploadTime) {}
    };

    std::vector<Attachment> attachments;

public:
    /**
     * @brief Constructor
     * @param intervention The intervention to decorate
     */
    explicit AttachmentsDecorator(std::unique_ptr<Intervention> intervention);

    /**
     * @brief Add a file attachment
     * @param filename Name of the file
     * @param description Optional description of the file
     * @return True if attachment was added successfully
     */
    bool addAttachment(const std::string& filename, const std::string& description = "");

    /**
     * @brief Remove an attachment
     * @param filename Name of the file to remove
     * @return True if attachment was removed successfully
     */
    bool removeAttachment(const std::string& filename);

    /**
     * @brief Get all attachments
     * @return Vector of attachment filenames
     */
    std::vector<std::string> getAttachmentFilenames() const;

    /**
     * @brief Check if a file is attached
     * @param filename Name of the file to check
     * @return True if the file is attached
     */
    bool hasAttachment(const std::string& filename) const;

    /**
     * @brief Get the number of attachments
     * @return Number of attachments
     */
    size_t getAttachmentCount() const;

    /**
     * @brief Override getInfo to include attachment information
     * @return Formatted string with intervention details including attachments
     */
    std::string getInfo() const override;
};

#endif // ATTACHMENTS_DECORATOR_H