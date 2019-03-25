from bs4 import BeautifulSoup


def filter_text_from_tags(rtw_text):
    """
    Function clean-ups text from rich text widget.
    :param rtw_text: text from rich text widget;
    :type rtw_text: str;
    :return: prepeared text;
    :rtype: str.
    """
    clear_text = BeautifulSoup(rtw_text, "html.parser").text
    return clear_text


def table_to_dict(table, rows_to_delete=None):
    """
    Creates a dict from table getting rid of useless rows.
    :param table: a table;
    :param rows_to_delete: list of rows to additionally delete from table;
    :return: table dict;
    :type table: sqlalchemy table;
    :type rows_to_delete: list of str;
    :rtype: dict of strings to strings.
    """
    if rows_to_delete is None:
        rows_to_delete = []

    result = table.__dict__
    result.pop('_sa_instance_state')
    for row in rows_to_delete:
        result.pop(row)

    return result
