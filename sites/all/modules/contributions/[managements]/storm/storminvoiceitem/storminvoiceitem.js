function stormtask_project_tasks(_project_select, _task_select_id, _with_all_option, _all_text) {
  var task_select = $("#" + _task_select_id).get(0);
  storm_empty_select(task_select);
  var project_nid = _project_select.value;
  if (!project_nid) project_nid=0;

  $.ajax({
    type: "GET",
    async: true,
    url: Drupal.settings.storm.project_tasks_url + Drupal.encodeURIComponent(project_nid),
    dataType: "string",
    success: function (data) {
      var items = Drupal.parseJson(data);
      storm_fill_select(task_select, items, _with_all_option, _all_text);
    }
  });
};

function stormtask_organization_project_tasks(_organization_select, _project_select_id, _task_select_id, _with_all_option, _all_text) {
  stormproject_organization_projects(_organization_select, _project_select_id, _with_all_option, _all_text);
  var project_select = $("#" + _project_select_id).get(0);
  stormtask_project_tasks(project_select, _task_select_id, _with_all_option, _all_text);
};

