
% Parameters
N = 128;
fov_x = 60;
fov_y = 60;

% Create coordinates
[a,el] = meshgrid(linspace(0, fov_x, N+1), linspace(-fov_y/2, fov_y/2, N+1));

% Convert to vector
az = a(:);
ele = el(:);

% Third coordinate has to be added
r = ones(numel(X), 1);


ele = ele ./ 180 .* pi;
az = az ./ 180 .* pi;


% Convert spherical coordinates to cartesian
X = r .* cos(ele) .* cos(az);
Y = r .* cos(ele) .* sin(az);
Z = r .* sin(ele);
    
%[X, Y, Z] = sph2cart(az ./ 180 .* pi, ele ./ 180 .* pi, r);

%scatter3(X,Y,Z); return;


% Concentrate it in one array
plane_points = [X Y Z];


% Create faces which are all quadrates
faces = zeros(N*N, 4);
for i = 1:N
    for j = 1:N
        x1 = (i-1) * (N+1) + 1;
        x2 = i * (N+1) + 1;
        
        faces(j + (i-1) * N, :) = [x1+j-1, x1+j, x2+j, x2+j-1]';
    end
end


% Write it to obj file
write_obj_file('spherical_calotte.obj', plane_points / max(plane_points(:)), faces);
