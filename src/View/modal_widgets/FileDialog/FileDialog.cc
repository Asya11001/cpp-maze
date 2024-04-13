#include "FileDialog.h"

#include "View/common/macro.h"
#include "View/styles/styles.h"

namespace s21 {
FileDialog::FileDialog(const int width, const int height) {
  setFixedSize(width, height);
  setMinimumSize(640, 480);

  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  setWindowTitle(FILE_DIALOG_WINDOW_TITLE);
  auto* layout = new QVBoxLayout(this);

  file_model_ = new QFileSystemModel(this);

  SetNameFilters({""});

  tree_view_ = new QTreeView(this);

  tree_view_->setModel(file_model_);
  tree_view_->setRootIndex(file_model_->index(QDir::homePath()));
  QHeaderView* header = tree_view_->header();

  int border_width = 50;
  int border_count = 2;
  int header_width = width - (border_width * border_count);

  file_path_line_edit_ = new QLineEdit(this);
  // file_path_line_edit_->setEnabled(false);

  select_button_ = new QPushButton(FILE_DIALOG_SELECT_BUTTON_TITLE, this);
  select_button_->setObjectName(STRINGIFY(selectButton));
  cancel_button_ = new QPushButton(FILE_DIALOG_CANCEL_BUTTON_TITLE, this);
  cancel_button_->setObjectName(STRINGIFY(cancelButton));

  select_button_->setEnabled(false);

  layout->addWidget(tree_view_);
  layout->addWidget(file_path_line_edit_);
  layout->addWidget(select_button_);
  layout->addWidget(cancel_button_);

  UpdateStyles();

  header->resizeSection(0, header_width * 0.70);
  header->resizeSection(1, header_width * 0.10);
  header->resizeSection(2, header_width * 0.10);
  header->resizeSection(3, header_width * 0.10);

  connect(tree_view_, &QTreeView::clicked, this,
          &FileDialog::OnTreeViewClicked);
  connect(file_path_line_edit_, &QLineEdit::textChanged, this,
          &FileDialog::OnTextChanged);

  connect(select_button_, &QPushButton::clicked, &EventHandler::Get(),
          &EventHandler::FileDialogSelectButtonClicked);
  connect(&EventHandler::Get(),
          &EventHandler::EventFileDialogSelectButtonClicked, this,
          &FileDialog::SelectButtonClicked);

  // connect(this, &FileDialog::EventSelectButtonClicked, &EventHandler::Get(),
  //         &EventHandler::FileDialogOpenFile);

  // connect(this, &FileDialog::EventSelectButtonClicked,
  // &EventHandler::Get(), &EventHandler::FileIsSelected);

  connect(cancel_button_, &QPushButton::clicked, &EventHandler::Get(),
          &EventHandler::FileDialogCloseWindow);
  connect(&EventHandler::Get(), &EventHandler::EventFileDialogCloseWindow, this,
          &FileDialog::CloseWindow);
}

QString FileDialog::GetSelectedPath() const {
  return file_path_line_edit_->text();
}

QString FileDialog::GetSelectedFileName() const {
  const QFileInfo file_info(GetSelectedPath());
  return file_info.fileName();
}

void FileDialog::UpdateStyles() {
  this->setStyleSheet(GetStylesFromFile(PATH_FILE_DIALOG_STYLES_FILE));
  tree_view_->header()->setStyleSheet(
      GetStylesFromFile(PATH_FILE_DIALOG_STYLES_FILE));
}

void FileDialog::SetSize(const int weight, const int height) {
  this->resize(weight, height);
}

void FileDialog::SetRootPath(const QString& path) {
  file_model_->setRootPath(path);
}

void FileDialog::SelectFile() { accept(); }

void FileDialog::CloseWindow() { this->close(); }
}  // namespace s21
