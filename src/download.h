#ifndef DOWNLOAD_H
#define DOWNLOAD_H

static size_t write_callback_download(char* buf, size_t size, size_t nmemb, void* up);
int get_html(char* download_url, int option);
int download_gallery(char* gallery_id, int gallery_index, char* download_directory, char* download_extension);

#endif // DOWNLOAD_H