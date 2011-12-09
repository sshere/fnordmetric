class FnordMetric::Logger

  def self.start(logfile_path)    
    require 'json'
    event_ids = Queue.new
    dump_file = File.open(logfile_path, 'a+')

    fetcher = Thread.new do
      redis = Redis.new
      loop do
        event_id = event_ids.pop
        event_data = redis.get("fnordmetric-event-#{event_id}")        
        event_hash = JSON.parse(event_data) rescue next

        event_hash.merge!(:_time => Time.now.to_i)

        dump_file.write(event_hash.to_json+"\n")
        dump_file.flush

        print "\033[1;34m" 
        print event_hash.inspect
        print "\033[0m\n"             
      end
    end

    listener = Thread.new do  
      redis = Redis.new
      redis.subscribe("fnordmetric-announce") do |on|
        on.message do |channel, event_id|      
          event_ids << event_id
        end
      end
    end

    fetcher.join
  end

end