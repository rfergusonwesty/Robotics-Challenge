%% Summary
%
%The following code will plot data points in real time from a user defined
%number of sources. The x-axis will extend to either a user defined value
%or will shift in time. Change the three parameters below to suit your
%needs.
%
%Authored by Adam St. Amand

%% User Set Parameters

%Serial Port works for different platforms. Below are EXAMPLES of the 
%FORMAT for each platform. User input will likely need to be adjusted for 
%specific machine.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Linux and Linux 64            Serial Port = '/dev/ttyS0'
%Mac OS X 64                   Serial Port = '/dev/tty.KeySerial1'
%Windows 32 and Windows 64     Serial Port = 'com1'
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

s = serial('com5')


%Select the maximum value for the x-axis. '0' sets x-axis to dynamic
%meaning it will change with time.

<<<<<<< HEAD
limit = 0;    %0 = Dynamic, ~0 = Static

=======
<<<<<<< HEAD
mode = 0;    %0 = Dynamic, ~0 = Static
=======
<<<<<<< HEAD
limit = 0;    %0 = Dynamic, ~0 = Static
=======
mode = 180;    %0 = Dynamic, ~0 = Static
>>>>>>> ae54704d78257665225721d6d56aaeb2a12d08d3
>>>>>>> develop
>>>>>>> ba8ddc54ad943c135ec4e2d7c9a9fe99a5b207b6


%Select how many sources of data are being plotted

magnitude = 3;

%% Initialize Variables

fopen(s);  %Initialize Serial Communication
error = 0; Y = 0; X = 0; count = 1;  % Initial Variables


%% Set up the figure 

figureHandle = figure('NumberTitle','off',...
    'Name','Real Time Data',...
    'Color',[0 0 0],'Visible','off');

% Set axes
axesHandle = axes('Parent',figureHandle,...
    'YGrid','on',...
    'YColor',[0.9725 0.9725 0.9725],...
    'XGrid','on',...
    'XColor',[0.9725 0.9725 0.9725],...
    'Color',[.1 .1 .1]);

hold on;

for i=1:magnitude
    if (limit~=0)
        plotHandle(i) = plot(axesHandle,X,Y,'.','DisplayName', strcat('Figure ',num2str(i)),'Color',[rand, rand, 1]);
    end
    
    if (limit==0)
         plotHandle(i) = plot(axesHandle,X,Y,'.-','DisplayName', strcat('Figure ',num2str(i)),'Color',[rand, rand, 1]);
    end
end

xlim(axesHandle,[min(0) max(limit)+1]);

% Create xlabel
xlabel('X','FontWeight','bold','FontSize',14,'Color',[.8 .8 .8]);

% Create ylabel
ylabel('Y','FontWeight','bold','FontSize',14,'Color',[.8 .8 .8]);

% Create title
title('Serial Output','FontSize',15,'Color',[1 1 1]);

% Create Legend
h=legend(gca,'show','Location','southoutside');
set(h,'TextColor', [1 1 1])




%% Reads values and plots them 

try  % Everything within 'try' will execute as long as there are no errors
    
while (1==1)
    for i=1:magnitude
        if (limit~=0)
            index = fscanf(s,'%f');
            X(index+1,i) = index;
            Y(index+1,i) = fscanf(s,'%f');
        end
        
        if (limit==0)
             X(count+1,i) = count;
             Y(count+1,i) = fscanf(s,'%f');
        end
        
        set(plotHandle(i),'YData',Y(:,i),'XData',X(:,i));
        set(figureHandle,'Visible','on');
    end
    
    if (limit == 0)
        xlim(axesHandle,[count-100 count+.01]);
    end
    
    pause(.001);
    count = count +1;
    
end


% If there was an error above, it will skip down to here.
catch ME
    error = 1;
    errorMessage = sprintf('Error in function RealTimePlot.\n\nError Message: %s', ME.message);
	fprintf(1,'%s', errorMessage);
    fprintf('\nSerial Port has been closed\n')
end


%% Clean up the serial port

fclose(s);
delete(s);
clear s;
