// Place all the behaviors and hooks related to the matching controller here.
// All this logic will automatically be available in application.js.


	$(function() {


		var points_list =  $('#data-result-search').find('.point');	
		var d = new Array();
		
		for(var i = 0; i < points_list.length; ++i) {
			var point = [0,0];
			point[0] = parseInt(points_list.eq(i).find('input[name=date]').val()) * 1000;
			point[1] = points_list.eq(i).find('input[name=value]').val();
			d.push(point);
		}

		// helper for returning the weekends in a period

		function weekendAreas(axes) {

			var markings = [],
				d = new Date(axes.xaxis.min);

			// go to the first Saturday

			d.setUTCDate(d.getUTCDate() - ((d.getUTCDay() + 1) % 7))
			d.setUTCSeconds(0);
			d.setUTCMinutes(0);
			d.setUTCHours(0);

			var i = d.getTime();

			// when we don't set yaxis, the rectangle automatically
			// extends to infinity upwards and downwards

			do {
				markings.push({ xaxis: { from: i, to: i + 2 * 24 * 60 * 60 * 1000 } });
				i += 7 * 24 * 60 * 60 * 1000;
			} while (i < axes.xaxis.max);

			return markings;
		}

		var options = {
            series: {
                color: 'rgb(170, 130, 130)',
                lines: {
                    show: true,
                    lineWidth: 2
                },
                shadowSize: 0
            },
            color: 'rgb(255, 0, 0)',
			xaxis: {
				mode: "time",
				tickLength: 5
			},
			selection: {
                color: 'rgb(0,255,0)',
				mode: "x"
			},

			grid: {
				markings: weekendAreas
			}
		};

		var plot = $.plot("#placeholder", [d], options);

		var overview = $.plot("#overview", [d], {
			series: {
                color: 'rgb(130, 130, 130)',
                lines: {
					show: true,
					lineWidth: 2
				},
				shadowSize: 0
			},
			xaxis: {
				ticks: [],
				mode: "time"
			},
			yaxis: {
				ticks: [],
				min: 0,
				autoscaleMargin: 0.1
			},

			selection: {
                color: 'rgb(0,255,0)',
				mode: "x"
			}
		});

		// now connect the two

		$("#placeholder").bind("plotselected", function (event, ranges) {

			// do the zooming
			$.each(plot.getXAxes(), function(_, axis) {
				var opts = axis.options;
				opts.min = ranges.xaxis.from;
				opts.max = ranges.xaxis.to;
			});
			plot.setupGrid();
			plot.draw();
			plot.clearSelection();

			// don't fire event on the overview to prevent eternal loop

			overview.setSelection(ranges, true);
		});

		$("#overview").bind("plotselected", function (event, ranges) {
			plot.setSelection(ranges);
		});

		// Add the Flot version string to the footer

		$("#footer").prepend("Flot " + $.plot.version + " &ndash; ");

	});
