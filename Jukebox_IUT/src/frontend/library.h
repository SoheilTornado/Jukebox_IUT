#ifndef LIBRARY_H
#define LIBRARY_H

#include <QWidget>
#include <QStringList>

#include "../backend/libraryManager.h"


QT_BEGIN_NAMESPACE
namespace Ui { class library; }
QT_END_NAMESPACE

class library final : public QWidget {
Q_OBJECT

public:
    explicit library(QWidget *parent = nullptr);
    ~library() override;

private slots:
    void onSearch(const QString &searchTerm);
    void onAddButtonClicked() const;
    void onSearchInitiated();
    void onLibraryChanged();

signals:

private:
    Ui::library *ui;

    QVector<song> searchResults;
    bool noResults = false;

    void renderSongs();
};


#endif //LIBRARY_H
