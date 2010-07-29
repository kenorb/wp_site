function stormticket_task_tickets(_task_select, _organization_select_id, _project_select_id, _ticket_select_id, _with_all_option, _all_text) {
  var ticket_select = $("#" + _ticket_select_id).get(0);
  storm_empty_select(ticket_select);

  var organization_select = $("#" + _organization_select_id).get(0);
  var organization_nid = organization_select.value;
  if (!organization_nid) organization_nid=0;

  var project_select = $("#" + _project_select_id).get(0);
  var project_nid = project_select.value;
  if (!project_nid) project_nid=0;

  var task_nid = _task_select.value;
  if (!task_nid) task_nid=0;

  $.ajax({
    type: "GET",
    async: true,
    url: Drupal.settings.storm.task_tickets_url + Drupal.encodeURIComponent(organization_nid) + 
    '/' + Drupal.encodeURIComponent(project_nid) +
    '/' + Drupal.encodeURIComponent(task_nid),
    dataType: "string",
    success: function (data) {
      var items = Drupal.parseJson(data);
      storm_fill_select(ticket_select, items, _with_all_option, _all_text);
    }
  });
};

function stormticket_project_task_tickets(_project_select, _organization_select_id, _task_select_id, _ticket_select_id, _with_all_option, _all_text) {
  stormtask_project_tasks(_project_select, _task_select_id, _with_all_option, _all_text);
  var task_select = $("#" + _task_select_id).get(0);
  stormticket_task_tickets(task_select, _organization_select_id, _project_select.id, _ticket_select_id, _with_all_option, _all_text);
};

function stormticket_organization_project_task_tickets(_organization_select, _project_select_id, _task_select_id, _ticket_select_id, _with_all_option, _all_text) {
  stormproject_organization_projects(_organization_select, _project_select_id, _with_all_option, _all_text);
  var project_select = $("#" + _project_select_id).get(0);
  stormticket_project_task_tickets(project_select, _organization_select.id, _task_select_id, _ticket_select_id, _with_all_option, _all_text);
};

