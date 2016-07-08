// Copyright 2010-2014 RethinkDB, all rights reserved.
#ifndef CLUSTERING_ADMINISTRATION_JOBS_BACKEND_HPP_
#define CLUSTERING_ADMINISTRATION_JOBS_BACKEND_HPP_

#include <memory>
#include <string>
#include <vector>

#include "rdb_protocol/artificial_table/caching_cfeed_backend.hpp"
#include "clustering/administration/metadata.hpp"
#include "concurrency/watchable.hpp"

class server_config_client_t;

class jobs_artificial_table_backend_t :
    public timer_cfeed_artificial_table_backend_t
{
public:
    jobs_artificial_table_backend_t(
        mailbox_manager_t *_mailbox_manager,
        std::shared_ptr<semilattice_readwrite_view_t<
            cluster_semilattice_metadata_t> > _semilattice_view,
        const clone_ptr_t<watchable_t<change_tracking_map_t<
            peer_id_t, cluster_directory_metadata_t> > > &_directory_view,
        server_config_client_t *_server_config_client,
        table_meta_client_t *_table_meta_client,
        admin_identifier_format_t _identifier_format);
    ~jobs_artificial_table_backend_t();

    std::string get_primary_key_name();

    bool read_all_rows_as_vector(signal_t *interruptor,
                                 std::vector<ql::datum_t> *rows_out,
                                 admin_err_t *error_out);

    bool read_row(ql::datum_t primary_key,
                  signal_t *interruptor,
                  ql::datum_t *row_out,
                  admin_err_t *error_out);

    bool write_row(ql::datum_t primary_key,
                   bool pkey_was_autogenerated,
                   ql::datum_t *new_value_inout,
                   signal_t *interruptor,
                   admin_err_t *error_out);

private:
    void get_all_job_reports(
            signal_t *interruptor,
            std::map<uuid_u, ql::datum_t> *jobs_out);

    mailbox_manager_t *mailbox_manager;

    std::shared_ptr< semilattice_readwrite_view_t<
        cluster_semilattice_metadata_t> > semilattice_view;

    clone_ptr_t<watchable_t<change_tracking_map_t<peer_id_t,
        cluster_directory_metadata_t> > > directory_view;

    server_config_client_t *server_config_client;
    table_meta_client_t *table_meta_client;

    admin_identifier_format_t identifier_format;
};

#endif /* CLUSTERING_ADMINISTRATION_JOBS_BACKEND_HPP_ */
