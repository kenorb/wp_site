function storminvoice_project_invoices(_project_select, _organization_select_id, _invoice_select_id, _with_all_option, _all_text) {
  var invoice_select = $("#" + _invoice_select_id).get(0);
  storm_empty_select(invoice_select);

  var organization_select = $("#" + _organization_select_id).get(0);
  var organization_nid = organization_select.value;
  if (!organization_nid) organization_nid=0;

  var project_nid = _project_select.value;
  if (!project_nid) project_nid=0;

  $.ajax({
    type: "GET",
    async: true,
    url: Drupal.settings.storm.project_invoices_url + Drupal.encodeURIComponent(organization_nid) +
    '/' + Drupal.encodeURIComponent(project_nid),
    dataType: "string",
    success: function (data) {
      var items = Drupal.parseJson(data);
      storm_fill_select(invoice_select, items, _with_all_option, _all_text);
    }
  });
};

function storminvoice_organization_project_invoices(_organization_select, _project_select_id, _invoice_select_id, _with_all_option, _all_text) {
  stormproject_organization_projects(_organization_select, _project_select_id, _with_all_option, _all_text);
  var project_select = $("#" + _project_select_id).get(0);
  storminvoice_project_invoices(project_select, _organization_select.id, _invoice_select_id, _with_all_option, _all_text);
};

