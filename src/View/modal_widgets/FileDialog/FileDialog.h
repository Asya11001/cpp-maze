#ifndef A1_MAZE_CPP_1_VIEW_MODAL_WIDGETS_FILE_DIALOG_FILE_DIALOG_H_
#define A1_MAZE_CPP_1_VIEW_MODAL_WIDGETS_FILE_DIALOG_FILE_DIALOG_H_

#include <QApplication>
#include <QDialog>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>

#include "View/EventHandler/EventHandler.h"

#define FILE_DIALOG_WINDOW_TITLE "Open File"
#define FILE_DIALOG_SELECT_BUTTON_TITLE "Select"
#define FILE_DIALOG_CANCEL_BUTTON_TITLE "Cancel"

namespace s21 {
class FileDialog final : public QDialog {
  Q_OBJECT

 public:
  FileDialog(int width, int height);

  [[nodiscard]] QString GetSelectedPath() const;

  [[nodiscard]] QString GetSelectedFileName() const;

  void SetSize(int weight, int height);

  void SetRootPath(const QString &path);

  void SelectButtonClicked() {
    accept();
    emit EventSelectButtonClicked(GetSelectedPath());
  }

  void SetNameFilters(const QStringList &filters) {
    allowed_extensions_ = filters;

    QStringList name_filters;
    for (const QString &extension : allowed_extensions_) {
      if (extension.isEmpty()) {
        name_filters << "*";
      } else {
        name_filters << "*" + extension;
      }
    }

    file_model_->setNameFilters(name_filters);
    file_model_->setNameFilterDisables(false);
  }

  void OnTreeViewClicked(const QModelIndex &index) {
    const QString selected_path =
        file_model_->fileInfo(index).absoluteFilePath();
    file_path_line_edit_->setText(selected_path);

    bool is_valid_extension = false;

    for (const QString &extension : allowed_extensions_) {
      if (extension == "") {
        if (QFileInfo(selected_path).isDir()) {
          is_valid_extension = true;

          break;
        }
      } else {
        if (selected_path.endsWith(extension, Qt::CaseInsensitive)) {
          is_valid_extension = true;
          break;
        }
      }
    }
    select_button_->setEnabled(is_valid_extension);
  }

  void OnTextChanged(const QString &string) {
    bool is_valid_extension = false;

    for (const QString &extension : allowed_extensions_) {
      if (extension == "") {
        if (QFileInfo(string).isDir()) {
          is_valid_extension = true;

          break;
        }
      } else {
        if (string.endsWith(extension, Qt::CaseInsensitive)) {
          is_valid_extension = true;
          break;
        }
      }
    }
    select_button_->setEnabled(is_valid_extension);
  }

 signals:
  void EventSelectButtonClicked(const QString &);

 public slots:
  void SelectFile();

  void CloseWindow();

 private:
  void UpdateStyles();

  QFileSystemModel *file_model_;
  QTreeView *tree_view_;
  QLineEdit *file_path_line_edit_;
  QPushButton *cancel_button_;
  QPushButton *select_button_;

  QStringList allowed_extensions_{};
};
}  // namespace s21
#endif  // A1_MAZE_CPP_1_VIEW_MODAL_WIDGETS_FILE_DIALOG_FILE_DIALOG_H_