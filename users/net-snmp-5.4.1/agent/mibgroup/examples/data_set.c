/**  @example data_set.c
 *  This example creates a table full of information and stores all
 *  that information within the agent's memory.  The "table_dataset"
 *  helper routines take care of handling all aspects of SNMP requests
 *  as they come in (yay!).
 *
 *  The exmaple we are instrumenting is an otherwise-useless table
 *  containing the names of IETF working group chairs.  Obviously,
 *  this data isn't all that useful from a network management point of
 *  view but this example only demonstrates how to use and store data.
 *  For more useful examples (but more complex), check out the
 *  apps/notification_log.c file which implements parts of the
 *  NOTIFICATION-LOG-MIB for logging incoming SNMP notifications.
 *
 *  Much of this code could be automatically generated by running
 *  mib2c as follows:
 *
 *    - mib2c -c mib2c.create-dataset.conf netSnmpIETFWGTable
 *
 *  The table is defined roughly as follows:
 *
 *  <pre>
 *    % snmptranslate -m NET-SNMP-EXAMPLES-MIB -Tp -IR netSnmpIETFWGTable
 *    |+--netSnmpIETFWGTable(1)
 *    |   |
 *    |   +--netSnmpIETFWGEntry(1)
 *    |      |  Index: nsIETFWGName
 *    |      |
 *    |      +-- ---- String    nsIETFWGName(1)
 *    |      |        Size: 1..32
 *    |      +-- CR-- String    nsIETFWGChair1(2)
 *    |      +-- CR-- String    nsIETFWGChair2(3)
 *  </pre>
 *
 *  If this module is compiled into an agent, you should be able to
 *  issue snmp commands that look something like (valid authentication
 *  information not shown in these commands):
 *
 *  <pre>
 *      % snmpwalk localhost netSnmpIETFWGTable
 *      nsIETFWGChair1."snmpv3" = "Russ Mundy"
 *      nsIETFWGChair2."snmpv3" = "David Harrington"
 *
 *      % snmpset localhost nsIETFWGChair1.\"sming\" = "David Durham"
 *      nsIETFWGChair1."sming" = "David Durham"
 *  
 *      % snmpwalk localhost netSnmpIETFWGTable
 *      nsIETFWGChair1."sming" = "David Durham"
 *      nsIETFWGChair1."snmpv3" = "Russ Mundy"
 *      nsIETFWGChair2."snmpv3" = "David Harrington"
 *
 *      In your snmpd.conf file, put the following line:
 *      add_row netSnmpIETFWGTable eos "Glenn Waters" "Dale Francisco"
 *
 *      % snmpwalk localhost netSnmpIETFWGTable
 *      nsIETFWGChair1.\"eos\" = "Glenn Waters"
 *      nsIETFWGChair1.\"snmpv3\" = "Russ Mundy"
 *      nsIETFWGChair2.\"eos\" = "Dale Francisco"
 *      nsIETFWGChair2.\"snmpv3\" = "David Harrington"
 *  </pre>
 */

/*
 * start be including the appropriate header files 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * our initialization routine, automatically called by the agent 
 */
/*
 * (to get called, the function name must match init_FILENAME() 
 */
void
init_data_set(void)
{
    netsnmp_table_data_set *table_set;
    netsnmp_table_row *row;

    /*
     * the OID we want to register our integer at.  This should be the
     * * OID node for the entire table.  In our case this is the
     * * netSnmpIETFWGTable oid definition 
     */
    oid             my_registration_oid[] =
        { 1, 3, 6, 1, 4, 1, 8072, 2, 2, 1 };

    /*
     * a debugging statement.  Run the agent with -Dexample_data_set to see
     * * the output of this debugging statement. 
     */
    DEBUGMSGTL(("example_data_set",
                "Initalizing example dataset table\n"));

    /*
     * It's going to be the "working group chairs" table, since I'm
     * * sitting at an IETF convention while I'm writing this.
     * *
     * *  column 1 = index = string = WG name
     * *  column 2 = string = chair #1
     * *  column 3 = string = chair #2  (most WGs have 2 chairs now)
     */

    table_set = netsnmp_create_table_data_set("netSnmpIETFWGTable");

    /*
     * allow the creation of new rows via SNMP SETs 
     */
    table_set->allow_creation = 1;

    /*
     * set up what a row "should" look like, starting with the index 
     */
    netsnmp_table_dataset_add_index(table_set, ASN_OCTET_STR);

    /*
     * define what the columns should look like.  both are octet strings here 
     */
    netsnmp_table_set_multi_add_default_row(table_set,
                                            /*
                                             * column 2 = OCTET STRING,
                                             * writable = 1,
                                             * default value = NULL,
                                             * default value len = 0 
                                             */
                                            2, ASN_OCTET_STR, 1, NULL, 0,
                                            /*
                                             * similar 
                                             */
                                            3, ASN_OCTET_STR, 1, NULL, 0,
                                            0 /* done */ );

    /*
     * register the table 
     */
    /*
     * if we wanted to handle specific data in a specific way, or note
     * * when requests came in we could change the NULL below to a valid
     * * handler method in which we could over ride the default
     * * behaviour of the table_dataset helper 
     */
    netsnmp_register_table_data_set(netsnmp_create_handler_registration
                                    ("netSnmpIETFWGTable", NULL,
                                     my_registration_oid,
                                     OID_LENGTH(my_registration_oid),
                                     HANDLER_CAN_RWRITE), table_set, NULL);


    /*
     * create the a row for the table, and add the data 
     */
    row = netsnmp_create_table_data_row();
    /*
     * set the index to the IETF WG name "snmpv3" 
     */
    netsnmp_table_row_add_index(row, ASN_OCTET_STR, "snmpv3",
                                strlen("snmpv3"));


    /*
     * set column 2 to be the WG chair name "Russ Mundy" 
     */
    netsnmp_set_row_column(row, 2, ASN_OCTET_STR,
                           "Russ Mundy", strlen("Russ Mundy"));
    netsnmp_mark_row_column_writable(row, 2, 1);        /* make writable via SETs */

    /*
     * set column 3 to be the WG chair name "David Harrington" 
     */
    netsnmp_set_row_column(row, 3, ASN_OCTET_STR, "David Harrington",
                           strlen("David Harrington"));
    netsnmp_mark_row_column_writable(row, 3, 1);        /* make writable via SETs */

    /*
     * add the row to the table 
     */
    netsnmp_table_dataset_add_row(table_set, row);

#ifdef ADD_MORE_DATA
    /*
     * add the data, for the second row 
     */
    row = netsnmp_create_table_data_row();
    netsnmp_table_row_add_index(row, ASN_OCTET_STR, "snmpconf",
                                strlen("snmpconf"));
    netsnmp_set_row_column(row, 2, ASN_OCTET_STR, "David Partain",
                           strlen("David Partain"));
    netsnmp_mark_row_column_writable(row, 2, 1);        /* make writable */
    netsnmp_set_row_column(row, 3, ASN_OCTET_STR, "Jon Saperia",
                           strlen("Jon Saperia"));
    netsnmp_mark_row_column_writable(row, 3, 1);        /* make writable */
    netsnmp_table_dataset_add_row(table_set, row);
#endif

    /*
     * Finally, this actually allows the "add_row" token it the
     * * snmpd.conf file to add rows to this table.
     * * Example snmpd.conf line:
     * *   add_row netSnmpIETFWGTable eos "Glenn Waters" "Dale Francisco"
     */
    netsnmp_register_auto_data_table(table_set, NULL);

    DEBUGMSGTL(("example_data_set", "Done initializing.\n"));
}
