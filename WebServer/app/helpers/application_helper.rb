module ApplicationHelper
   def error_messages_for(object)
    render 'layouts/error_messages_for', object: object
  end


  def link_to_add_fields(name, f, association)
    new_object= f.object.class.reflect_on_association(association).klass.new
    fields= f.fields_for(association,new_object, child_index: "new_#{association}") do |builder|
      render association.to_s.singularize+"_fields", f: builder
    end
    link_to(name,?#, class:"add_fields btn btn-warning", data:{body: "#{fields}", association:association})
  end

  def link_to_remove_fields(f)
    f.hidden_field(:destroy, data: {destroy:1})+link_to(?x, ?#, :class => "remove_fields btn btn_danger")
  end

  def selectify(arr)
    res=[]
    for i in 0...arr.size
      res << [arr[i],i]
    end
    res
  end

  def link_to_back
  link_to(fa_icon("angle-left")+' Назад',:back, class: 'btn btn-default')
  end
end
